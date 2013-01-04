/****************************************************************************
*                                                                           *
*  OpenNI 1.x Alpha                                                         *
*  Copyright (C) 2011 PrimeSense Ltd.                                       *
*                                                                           *
*  This file is part of OpenNI.                                             *
*                                                                           *
*  OpenNI is free software: you can redistribute it and/or modify           *
*  it under the terms of the GNU Lesser General Public License as published *
*  by the Free Software Foundation, either version 3 of the License, or     *
*  (at your option) any later version.                                      *
*                                                                           *
*  OpenNI is distributed in the hope that it will be useful,                *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
*  GNU Lesser General Public License for more details.                      *
*                                                                           *
*  You should have received a copy of the GNU Lesser General Public License *
*  along with OpenNI. If not, see <http://www.gnu.org/licenses/>.           *
*                                                                           *
****************************************************************************/
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "SceneDrawer.h"

#ifndef USE_GLES
#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif
#else
    #include "opengles.h"
#endif

extern xn::UserGenerator g_UserGenerator;
extern xn::DepthGenerator g_DepthGenerator;

extern XnBool g_bDrawBackground;
extern XnBool g_bDrawPixels;
extern XnBool g_bDrawSkeleton;
extern XnBool g_bPrintID;
extern XnBool g_bPrintState;

#include <map>
std::map<XnUInt32, std::pair<XnCalibrationStatus, XnPoseDetectionStatus> > m_Errors;
void XN_CALLBACK_TYPE MyCalibrationInProgress(xn::SkeletonCapability& capability, XnUserID id, XnCalibrationStatus calibrationError, void* pCookie)
{
    m_Errors[id].first = calibrationError;
}
void XN_CALLBACK_TYPE MyPoseInProgress(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID id, XnPoseDetectionStatus poseError, void* pCookie)
{
    m_Errors[id].second = poseError;
}

#define MAX_DEPTH 10000
float g_pDepthHist[MAX_DEPTH];
unsigned int getClosestPowerOfTwo(unsigned int n)
{
    unsigned int m = 2;
    while(m < n) m<<=1;

    return m;
}
GLuint initTexture(void** buf, int& width, int& height)
{
    GLuint texID = 0;
    glGenTextures(1,&texID);

    width = getClosestPowerOfTwo(width);
    height = getClosestPowerOfTwo(height); 
    *buf = new unsigned char[width*height*4];
    glBindTexture(GL_TEXTURE_2D,texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texID;
}

GLfloat texcoords[8];
void DrawRectangle(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
    GLfloat verts[8] = {    topLeftX, topLeftY,
        topLeftX, bottomRightY,
        bottomRightX, bottomRightY,
        bottomRightX, topLeftY
    };
    glVertexPointer(2, GL_FLOAT, 0, verts);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    //TODO: Maybe glFinish needed here instead - if there's some bad graphics crap
    glFlush();
}
void DrawTexture(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

    DrawRectangle(topLeftX, topLeftY, bottomRightX, bottomRightY);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

XnFloat Colors[][3] =
{
    {0,1,1},
    {0,0,1},
    {0,1,0},
    {1,1,0},
    {1,0,0},
    {1,.5,0},
    {.5,1,0},
    {0,.5,1},
    {.5,0,1},
    {1,1,.5},
    {1,1,1}
};
XnUInt32 nColors = 10;
#ifndef USE_GLES
void glPrintString(void *font, char *str)
{
    int i,l = strlen(str);

    for(i=0; i<l; i++)
    {
        glutBitmapCharacter(font,*str++);
    }
}
#endif
inline void DrawLimb(XnUserID player, XnSkeletonJoint eJoint1, XnSkeletonJoint eJoint2)
{
    if (!g_UserGenerator.GetSkeletonCap().IsTracking(player))
    {
        printf("not tracked!\n");
        return;
    }

    XnSkeletonJointPosition joint1, joint2;
    g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, eJoint1, joint1);
    g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, eJoint2, joint2);

    if (joint1.fConfidence < 0.5 || joint2.fConfidence < 0.5)
    {
        return;
    }

    XnPoint3D pt[2];
    pt[0] = joint1.position;
    pt[1] = joint2.position;

    g_DepthGenerator.ConvertRealWorldToProjective(2, pt, pt);
#ifndef USE_GLES
    glVertex3i(pt[0].X, pt[0].Y, 0);
    glVertex3i(pt[1].X, pt[1].Y, 0);
#else
    GLfloat verts[4] = {pt[0].X, pt[0].Y, pt[1].X, pt[1].Y};
    glVertexPointer(2, GL_FLOAT, 0, verts);
    glDrawArrays(GL_LINES, 0, 2);
    glFlush();
#endif
}


inline void DrawCoordinateLines(unsigned int xRes, unsigned int yRes)
{

    //set color
    glColor4f(1, 1, 1, 1);

    //horizontal line
    GLfloat vertsHorizontal[4] = {xRes / 2 - xRes / 20, yRes / 2, xRes / 2 + xRes / 20, yRes / 2};
    glVertexPointer(2, GL_FLOAT, 0, vertsHorizontal);
    glDrawArrays(GL_LINES, 0, 2);

    //vertical line
    GLfloat vertsVertical[4] = {xRes / 2, yRes / 2  - yRes / 20, xRes / 2, yRes / 2 + yRes / 20};
    glVertexPointer(2, GL_FLOAT, 0, vertsVertical);
    glDrawArrays(GL_LINES, 0, 2);

    glFlush();

}


inline bool PointChecker(XnPoint3D* p_pt)
{
    const int cubeXDimension = 1000;
    const int cubeYDimension = 1000;
    const int cubeZDimension = 1000;

    const int cubeXCenter = 0;
    const int cubeYCenter = 0;
    const int cubeZCenter = 3260;

    bool xFlag = false;
    bool yFlag = false;
    bool zFlag = false;

    //check z dimension
    if (p_pt->Z > (cubeZCenter - cubeZDimension/2) && p_pt->Z < (cubeZCenter + cubeZDimension/2))
    {
//         printf(">>> player %u is in z!!!\n\n", player);
        zFlag = true;
    }

    //check y dimension
    if (p_pt->Y > (cubeYCenter - cubeYDimension/2) && p_pt->Y < (cubeYCenter + cubeYDimension/2))
    {
//         printf(">>> player %u is in y!!!\n\n", player);
        yFlag = true;
    }

    //check x dimension
    if (p_pt->X > (cubeXCenter - cubeXDimension/2) && p_pt->X < (cubeXCenter + cubeXDimension/2))
    {
//         printf(">>> player %u is in x!!!\n\n", player);
        xFlag = true;

    }

    return zFlag && yFlag && xFlag;

}



inline void InOutChecker(XnUserID player)
{
    static unsigned long counter = 0;

    XnSkeletonJointPosition jointRightHand, jointLeftHand, jointHead;
    bool rightHandCheck, leftHandCheck, headCheck;

    g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_HAND, jointRightHand);
    g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_HAND, jointLeftHand);
    g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_HEAD, jointHead);

    rightHandCheck = PointChecker(&jointRightHand.position);
    leftHandCheck = PointChecker(&jointLeftHand.position);
    headCheck = PointChecker(&jointHead.position);

    if (rightHandCheck || leftHandCheck || headCheck)
    {
        printf("**********************************************************************\n");
        printf("PLAYER %u IN CUBE!!!!\n", player);
        printf("**********************************************************************\n");

        if(rightHandCheck)
        {
            printf(" * jointRightHand.position (X: %f, Y: %f, Z: %f)\n", jointRightHand.position.X, jointRightHand.position.Y, jointRightHand.position.Z);
        }
        if(leftHandCheck)
        {
            printf(" * jointLeftHand.position (X: %f, Y: %f, Z: %f)\n", jointLeftHand.position.X, jointLeftHand.position.Y, jointLeftHand.position.Z);
        }
        if(headCheck)
        {
            printf(" * jointHead.position (X: %f, Y: %f, Z: %f)\n", jointHead.position.X, jointHead.position.Y, jointHead.position.Z);
        }

        printf("**********************************************************************\n\n");
    }


    //60: @30fps -> 2s
    if (counter % 60 == 0)
    {
        printf("**********************************************************************\n");
        printf("PLAYER %u POSITION INFORMATION\n", player);
        printf("**********************************************************************\n");

        printf(" * jointRightHand.position (X: %f, Y: %f, Z: %f)\n", jointRightHand.position.X, jointRightHand.position.Y, jointRightHand.position.Z);
        printf(" * jointLeftHand.position (X: %f, Y: %f, Z: %f)\n", jointLeftHand.position.X, jointLeftHand.position.Y, jointLeftHand.position.Z);
        printf(" * jointHead.position (X: %f, Y: %f, Z: %f)\n", jointHead.position.X, jointHead.position.Y, jointHead.position.Z);

        printf("**********************************************************************\n\n");
    }


    counter++;

}


const XnChar* GetCalibrationErrorString(XnCalibrationStatus error)
{
    switch (error)
    {
    case XN_CALIBRATION_STATUS_OK:
        return "OK";
    case XN_CALIBRATION_STATUS_NO_USER:
        return "NoUser";
    case XN_CALIBRATION_STATUS_ARM:
        return "Arm";
    case XN_CALIBRATION_STATUS_LEG:
        return "Leg";
    case XN_CALIBRATION_STATUS_HEAD:
        return "Head";
    case XN_CALIBRATION_STATUS_TORSO:
        return "Torso";
    case XN_CALIBRATION_STATUS_TOP_FOV:
        return "Top FOV";
    case XN_CALIBRATION_STATUS_SIDE_FOV:
        return "Side FOV";
    case XN_CALIBRATION_STATUS_POSE:
        return "Pose";
    default:
        return "Unknown";
    }
}


const XnChar* GetPoseErrorString(XnPoseDetectionStatus error)
{
    switch (error)
    {
    case XN_POSE_DETECTION_STATUS_OK:
        return "OK";
    case XN_POSE_DETECTION_STATUS_NO_USER:
        return "NoUser";
    case XN_POSE_DETECTION_STATUS_TOP_FOV:
        return "Top FOV";
    case XN_POSE_DETECTION_STATUS_SIDE_FOV:
        return "Side FOV";
    case XN_POSE_DETECTION_STATUS_ERROR:
        return "General error";
    default:
        return "Unknown";
    }
}


void DrawDepthMap(const xn::DepthMetaData& dmd, const xn::SceneMetaData& smd)
{
    static bool bInitialized = false;    
    static GLuint depthTexID;
    static unsigned char* pDepthTexBuf;
    static int texWidth, texHeight;

    float topLeftX;
    float topLeftY;
    float bottomRightY;
    float bottomRightX;
    float texXpos;
    float texYpos;

    if(!bInitialized)
    {
        texWidth =  getClosestPowerOfTwo(dmd.XRes());
        texHeight = getClosestPowerOfTwo(dmd.YRes());

//        printf("Initializing depth texture: width = %d, height = %d\n", texWidth, texHeight);
        depthTexID = initTexture((void**)&pDepthTexBuf,texWidth, texHeight) ;

//        printf("Initialized depth texture: width = %d, height = %d\n", texWidth, texHeight);
        bInitialized = true;

        topLeftX = dmd.XRes();
        topLeftY = 0;
        bottomRightY = dmd.YRes();
        bottomRightX = 0;
        texXpos =(float)dmd.XRes()/texWidth;
        texYpos  =(float)dmd.YRes()/texHeight;

        memset(texcoords, 0, 8*sizeof(float));
        texcoords[0] = texXpos, texcoords[1] = texYpos, texcoords[2] = texXpos, texcoords[7] = texYpos;
    }

    unsigned int nValue = 0;
    unsigned int nHistValue = 0;
    unsigned int nIndex = 0;
    unsigned int nX = 0;
    unsigned int nY = 0;
    unsigned int nNumberOfPoints = 0;
    XnUInt16 g_nXRes = dmd.XRes();
    XnUInt16 g_nYRes = dmd.YRes();

    unsigned char* pDestImage = pDepthTexBuf;

    const XnDepthPixel* pDepth = dmd.Data();
    const XnLabel* pLabels = smd.Data();

    // Calculate the accumulative histogram
    memset(g_pDepthHist, 0, MAX_DEPTH*sizeof(float));
    for (nY=0; nY<g_nYRes; nY++)
    {
        for (nX=0; nX<g_nXRes; nX++)
        {
            nValue = *pDepth;

            if (nValue != 0)
            {
                g_pDepthHist[nValue]++;
                nNumberOfPoints++;
            }

            pDepth++;
        }
    }

    for (nIndex=1; nIndex<MAX_DEPTH; nIndex++)
    {
        g_pDepthHist[nIndex] += g_pDepthHist[nIndex-1];
    }
    if (nNumberOfPoints)
    {
        for (nIndex=1; nIndex<MAX_DEPTH; nIndex++)
        {
            g_pDepthHist[nIndex] = (unsigned int)(256 * (1.0f - (g_pDepthHist[nIndex] / nNumberOfPoints)));
        }
    }

    pDepth = dmd.Data();
    if (g_bDrawPixels)
    {
        XnUInt32 nIndex = 0;
        // Prepare the texture map
        for (nY=0; nY<g_nYRes; nY++)
        {
            for (nX=0; nX < g_nXRes; nX++, nIndex++)
            {

                pDestImage[0] = 0;
                pDestImage[1] = 0;
                pDestImage[2] = 0;
                if (g_bDrawBackground || *pLabels != 0)
                {
                    nValue = *pDepth;
                    XnLabel label = *pLabels;
                    XnUInt32 nColorID = label % nColors;
                    if (label == 0)
                    {
                        nColorID = nColors;
                    }

                    if (nValue != 0)
                    {
                        nHistValue = g_pDepthHist[nValue];

                        pDestImage[0] = nHistValue * Colors[nColorID][0]; 
                        pDestImage[1] = nHistValue * Colors[nColorID][1];
                        pDestImage[2] = nHistValue * Colors[nColorID][2];
                    }
                }

                pDepth++;
                pLabels++;
                pDestImage+=3;
            }

            pDestImage += (texWidth - g_nXRes) *3;
        }
    }
    else
    {
        xnOSMemSet(pDepthTexBuf, 0, 3*2*g_nXRes*g_nYRes);
    }

    glBindTexture(GL_TEXTURE_2D, depthTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pDepthTexBuf);

    // Display the OpenGL texture map
    glColor4f(0.75,0.75,0.75,1);

    glEnable(GL_TEXTURE_2D);
    DrawTexture(dmd.XRes(),dmd.YRes(),0,0);
    glDisable(GL_TEXTURE_2D);

    char strLabel[50] = "";
    XnUserID aUsers[15];
    XnUInt16 nUsers = 15;
    g_UserGenerator.GetUsers(aUsers, nUsers);

    for (int i = 0; i < nUsers; ++i)
    {
#ifndef USE_GLES
        if (g_bPrintID)
        {
            XnPoint3D com;
            g_UserGenerator.GetCoM(aUsers[i], com);
            g_DepthGenerator.ConvertRealWorldToProjective(1, &com, &com);

            xnOSMemSet(strLabel, 0, sizeof(strLabel));
            if (!g_bPrintState)
            {
                // Tracking
                sprintf(strLabel, "%d", aUsers[i]);
            }
            else if (g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i]))
            {
                // Tracking
                sprintf(strLabel, "%d - Tracking", aUsers[i]);
            }
            else if (g_UserGenerator.GetSkeletonCap().IsCalibrating(aUsers[i]))
            {
                // Calibrating
                sprintf(strLabel, "%d - Calibrating [%s]", aUsers[i], GetCalibrationErrorString(m_Errors[aUsers[i]].first));
            }
            else
            {
                // Nothing
                sprintf(strLabel, "%d - Looking for pose [%s]", aUsers[i], GetPoseErrorString(m_Errors[aUsers[i]].second));
            }


            glColor4f(1-Colors[i%nColors][0], 1-Colors[i%nColors][1], 1-Colors[i%nColors][2], 1);

            glRasterPos2i(com.X, com.Y);
            glPrintString(GLUT_BITMAP_HELVETICA_18, strLabel);
        }
#endif
        if (g_bDrawSkeleton && g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i]))
        {
#ifndef USE_GLES
            glBegin(GL_LINES);
#endif
            glColor4f(1-Colors[aUsers[i]%nColors][0], 1-Colors[aUsers[i]%nColors][1], 1-Colors[aUsers[i]%nColors][2], 1);
            DrawLimb(aUsers[i], XN_SKEL_HEAD, XN_SKEL_NECK);

            DrawLimb(aUsers[i], XN_SKEL_NECK, XN_SKEL_LEFT_SHOULDER);
            DrawLimb(aUsers[i], XN_SKEL_LEFT_SHOULDER, XN_SKEL_LEFT_ELBOW);
            DrawLimb(aUsers[i], XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_HAND);

            DrawLimb(aUsers[i], XN_SKEL_NECK, XN_SKEL_RIGHT_SHOULDER);
            DrawLimb(aUsers[i], XN_SKEL_RIGHT_SHOULDER, XN_SKEL_RIGHT_ELBOW);
            DrawLimb(aUsers[i], XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_HAND);

            DrawLimb(aUsers[i], XN_SKEL_LEFT_SHOULDER, XN_SKEL_TORSO);
            DrawLimb(aUsers[i], XN_SKEL_RIGHT_SHOULDER, XN_SKEL_TORSO);

            DrawLimb(aUsers[i], XN_SKEL_TORSO, XN_SKEL_LEFT_HIP);
            DrawLimb(aUsers[i], XN_SKEL_LEFT_HIP, XN_SKEL_LEFT_KNEE);
            DrawLimb(aUsers[i], XN_SKEL_LEFT_KNEE, XN_SKEL_LEFT_FOOT);

            DrawLimb(aUsers[i], XN_SKEL_TORSO, XN_SKEL_RIGHT_HIP);
            DrawLimb(aUsers[i], XN_SKEL_RIGHT_HIP, XN_SKEL_RIGHT_KNEE);
            DrawLimb(aUsers[i], XN_SKEL_RIGHT_KNEE, XN_SKEL_RIGHT_FOOT);

            DrawLimb(aUsers[i], XN_SKEL_LEFT_HIP, XN_SKEL_RIGHT_HIP);


            //check if user is in cube or not
//             InOutChecker(aUsers[i]);


/*
            //send data once per frame... only player 0 and only if tracked!
            if (i == 0)
            {

                

                
            }*/

            

#ifndef USE_GLES
            glEnd();
#endif
        }


    }

    //draw coordinates in each frame
    DrawCoordinateLines(g_nXRes, g_nYRes);

//     DrawClickLeftLine()

}
