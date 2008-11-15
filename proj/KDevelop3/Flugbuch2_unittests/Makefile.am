
bin_PROGRAMS = Flugbuch2_qt Flugbuch2_unittests Flugbuch2_wt
Flugbuch2_unittests_SOURCES = CoreStructures.cpp CoreStructures.h \
	FlightDatabase.cpp FlightDatabase.h FlugbuchMain.cpp FormatStr.h MainExternalIncludes.h \
	inout.h inout_xml.cpp inout_xml.h
Flugbuch2_unittests_LDADD = -lboost_date_time-mt-d -lboost_serialization-mt-d \
	-lboost_unit_test_framework-mt-d
Flugbuch2_wt_SOURCES = CoreStructures.cpp CoreStructures.h FlightDatabase.cpp \
	FlightDatabase.h FlightList_wt.cpp FlightList_wt.h FormatStr.h MainExternalIncludes.h \
	inout.h inout_xml.cpp inout_xml.h main.cpp main.h
Flugbuch2_wt_LDADD = -lboost_date_time-mt-d-1_34_1 -lboost_serialization-mt-d \
	-lboost_signals-mt-d -lwt -lwtext -lwtfcgi -lwthttp
