#!/bin/bash
cd ~/ln/
#no.1: no error#
./rp-radio-nsc-noerror.sh CUBIC_NOERROR

#no.2: 46 noise, with error, no harq##
perl -pi -e 's/LteEnbPhy::TxPower \"5\"/LteEnbPhy::TxPower \"30\"/g' lte-nsc-in.txt
perl -pi -e 's/LteUePhy::NoiseFigure \"9\"/LteUePhy::NoiseFigure \"46\"/g' lte-nsc-in.txt
perl -pi -e 's/LteUePhy::TxPower \"50\"/LteUePhy::TxPower \"90\"/g' lte-nsc-in.txt
perl -pi -e 's/LteEnbPhy::NoiseFigure \"37\"/LteEnbPhy::NoiseFigure \"1\"/g' lte-nsc-in.txt
perl -pi -e 's/DataErrorModelEnabled \"false\"/DataErrorModelEnabled \"true\"/g' lte-nsc-in.txt	#with error
./rp-radio-nsc.sh CUBIC_46_NOHARQ


#no.3: 46 noise, with error, WITH harq##
perl -pi -e 's/HarqEnabled \"false\"/HarqEnabled \"true\"/g' lte-nsc-in.txt	#with harq
./rp-radio-nsc.sh CUBIC_46_WITHHARQ



#no.4: 46.1 noise, with error, WITH harq##
perl -pi -e 's/LteUePhy::NoiseFigure \"46\"/LteUePhy::NoiseFigure \"46.1\"/g' lte-nsc-in.txt
./rp-radio-nsc.sh CUBIC_461_WITHHARQ

#no.5: 46.1 noise, with error, no harq##
perl -pi -e 's/HarqEnabled \"true\"/HarqEnabled \"false\"/g' lte-nsc-in.txt	#without harq
./rp-radio-nsc.sh CUBIC_461_NOHARQ


#no.6: 46.2 noise, with error, WITHOUT harq##
perl -pi -e 's/LteUePhy::NoiseFigure \"46.1\"/LteUePhy::NoiseFigure \"46.2\"/g' lte-nsc-in.txt
./rp-radio-nsc.sh CUBIC_462_NOHARQ

#no.7: 46.2 noise, with error, WITH harq##
perl -pi -e 's/HarqEnabled \"false\"/HarqEnabled \"true\"/g' lte-nsc-in.txt	#with harq
./rp-radio-nsc.sh CUBIC_462_WITHHARQ

#no.8: 46.4 noise, with error, WITH harq##
perl -pi -e 's/LteUePhy::NoiseFigure \"46.2\"/LteUePhy::NoiseFigure \"46.4\"/g' lte-nsc-in.txt
./rp-radio-nsc.sh CUBIC_464_WITHHARQ

#no.9: 46.4 noise, with error, WITHOUT harq##
perl -pi -e 's/HarqEnabled \"false\"/HarqEnabled \"true\"/g' lte-nsc-in.txt	#with harq
./rp-radio-nsc.sh CUBIC_464_NOHARQ


