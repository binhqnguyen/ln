#!/bin/bash
cd ~/ln/
#no.1: no error#
./rp-radio-nsc-full.sh CUBIC_PED_9_10UE


#no.2: with error, no harq##
#perl -pi -e 's/DataErrorModelEnabled \"false\"/DataErrorModelEnabled \"true\"/g' lte-nsc-full-in.txt	
#./rp-radio-nsc-full.sh ERROR_F_461


#no.3: pedestrian mobility
perl -pi -e 's/IS_PEDESTRIAN=\"1\"/IS_PEDESTRIAN=\"0\"/g' rp-radio-nsc-full.sh	
./rp-radio-nsc-full.sh CUBIC_VEH_9_10UE

perl -pi -e 's/TCP_VERSION=\"cubic\"/TCP_VERSION=\"reno\"/g' rp-radio-nsc-full.sh	
./rp-radio-nsc-full.sh RENO_VEH_9_10UE


#perl -pi -e 's/HarqEnabled \"false\"/HarqEnabled \"true\"/g' lte-nsc-full-in.txt	#with harq
#./rp-radio-nsc-full.sh ERROR_PEDESTRIAN_FADING_HARQ_F_461

perl -pi -e 's/IS_PEDESTRIAN=\"0\"/IS_PEDESTRIAN=\"1\"/g' rp-radio-nsc-full.sh	
./rp-radio-nsc-full.sh RENO_PED_9_10UE





####SACK enabled######
perl -pi -e 's/SACK=\"0\"/SACK=\"1\"/g' rp-radio-nsc-full.sh	
perl -pi -e 's/TCP_VERSION=\"reno\"/TCP_VERSION=\"cubic\"/g' rp-radio-nsc-full.sh	

./rp-radio-nsc-full.sh CUBIC_PED_9_10UE_SACK


#no.3: pedestrian mobility
perl -pi -e 's/IS_PEDESTRIAN=\"1\"/IS_PEDESTRIAN=\"0\"/g' rp-radio-nsc-full.sh	
./rp-radio-nsc-full.sh CUBIC_VEH_9_10UE_SACK

perl -pi -e 's/TCP_VERSION=\"cubic\"/TCP_VERSION=\"reno\"/g' rp-radio-nsc-full.sh	
./rp-radio-nsc-full.sh RENO_VEH_9_10UE_SACK


#perl -pi -e 's/HarqEnabled \"false\"/HarqEnabled \"true\"/g' lte-nsc-full-in.txt	#with harq
#./rp-radio-nsc-full.sh ERROR_PEDESTRIAN_FADING_HARQ_F_461

perl -pi -e 's/IS_PEDESTRIAN=\"0\"/IS_PEDESTRIAN=\"1\"/g' rp-radio-nsc-full.sh	
./rp-radio-nsc-full.sh RENO_PED_9_10UE_SACK





####SACK disable, Window Scaling enabled######
perl -pi -e 's/SACK=\"1\"/SACK=\"0\"/g' rp-radio-nsc-full.sh	
perl -pi -e 's/WINDOW_SCALING=\"0\"/WINDOW_SCALING=\"1\"/g' rp-radio-nsc-full.sh	
perl -pi -e 's/TCP_VERSION=\"reno\"/TCP_VERSION=\"cubic\"/g' rp-radio-nsc-full.sh	

./rp-radio-nsc-full.sh CUBIC_PED_9_10UE_WSCAL


#no.3: pedestrian mobility
perl -pi -e 's/IS_PEDESTRIAN=\"1\"/IS_PEDESTRIAN=\"0\"/g' rp-radio-nsc-full.sh	
./rp-radio-nsc-full.sh CUBIC_VEH_9_10UE_WSCAL

perl -pi -e 's/TCP_VERSION=\"cubic\"/TCP_VERSION=\"reno\"/g' rp-radio-nsc-full.sh	
./rp-radio-nsc-full.sh RENO_VEH_9_10UE_WSCAL


#perl -pi -e 's/HarqEnabled \"false\"/HarqEnabled \"true\"/g' lte-nsc-full-in.txt	#with harq
#./rp-radio-nsc-full.sh ERROR_PEDESTRIAN_FADING_HARQ_F_461

perl -pi -e 's/IS_PEDESTRIAN=\"0\"/IS_PEDESTRIAN=\"1\"/g' rp-radio-nsc-full.sh	
./rp-radio-nsc-full.sh RENO_PED_9_10UE_WSCAL




####SACK enabled, Window Scaling enabled######
perl -pi -e 's/SACK=\"0\"/SACK=\"1\"/g' rp-radio-nsc-full.sh	
perl -pi -e 's/TCP_VERSION=\"reno\"/TCP_VERSION=\"cubic\"/g' rp-radio-nsc-full.sh	

./rp-radio-nsc-full.sh CUBIC_PED_9_10UE_SACK_WSCAL


#no.3: pedestrian mobility
perl -pi -e 's/IS_PEDESTRIAN=\"1\"/IS_PEDESTRIAN=\"0\"/g' rp-radio-nsc-full.sh	
./rp-radio-nsc-full.sh CUBIC_VEH_9_10UE_SACK_WSCAL

perl -pi -e 's/TCP_VERSION=\"cubic\"/TCP_VERSION=\"reno\"/g' rp-radio-nsc-full.sh	
./rp-radio-nsc-full.sh RENO_VEH_9_10UE_SACK_WSCAL


#perl -pi -e 's/HarqEnabled \"false\"/HarqEnabled \"true\"/g' lte-nsc-full-in.txt	#with harq
#./rp-radio-nsc-full.sh ERROR_PEDESTRIAN_FADING_HARQ_F_461

perl -pi -e 's/IS_PEDESTRIAN=\"0\"/IS_PEDESTRIAN=\"1\"/g' rp-radio-nsc-full.sh	
./rp-radio-nsc-full.sh RENO_PED_9_10UE_SACK_WSCAL


