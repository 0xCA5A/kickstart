include Common/CommonDefs.mak

.PHONY: all


all: UserTrackerMouseRemote



.PHONY: UserTrackerMouseRemote
UserTrackerMouseRemote:
	$(MAKE) -C ../UserTrackerMouseRemote


clean:
	touch ../Bin; rm -rf ../Bin
	$(MAKE) clean -C ../UserTrackerMouseRemote


	touch ../UserTrackerMouseRemote/x64-Release; rm -rf ../UserTrackerMouseRemote/x64-Release
