#! /usr/bin/env python

# Programs that are runnable.
ns3_runnable_programs = ['build/src/aodv/examples/ns3-dev-aodv-debug', 'build/src/bridge/examples/ns3-dev-csma-bridge-debug', 'build/src/bridge/examples/ns3-dev-csma-bridge-one-hop-debug', 'build/src/buildings/examples/ns3-dev-buildings-pathloss-profiler-debug', 'build/src/config-store/examples/ns3-dev-config-store-save-debug', 'build/src/core/examples/ns3-dev-main-callback-debug', 'build/src/core/examples/ns3-dev-sample-simulator-debug', 'build/src/core/examples/ns3-dev-main-ptr-debug', 'build/src/core/examples/ns3-dev-main-random-variable-debug', 'build/src/core/examples/ns3-dev-main-random-variable-stream-debug', 'build/src/core/examples/ns3-dev-sample-random-variable-debug', 'build/src/core/examples/ns3-dev-sample-random-variable-stream-debug', 'build/src/core/examples/ns3-dev-main-test-sync-debug', 'build/src/csma/examples/ns3-dev-csma-one-subnet-debug', 'build/src/csma/examples/ns3-dev-csma-broadcast-debug', 'build/src/csma/examples/ns3-dev-csma-packet-socket-debug', 'build/src/csma/examples/ns3-dev-csma-multicast-debug', 'build/src/csma/examples/ns3-dev-csma-raw-ip-socket-debug', 'build/src/csma/examples/ns3-dev-csma-ping-debug', 'build/src/csma-layout/examples/ns3-dev-csma-star-debug', 'build/src/dsdv/examples/ns3-dev-dsdv-manet-debug', 'build/src/dsr/examples/ns3-dev-dsr-debug', 'build/src/emu/examples/ns3-dev-emu-udp-echo-debug', 'build/src/emu/examples/ns3-dev-emu-ping-debug', 'build/src/energy/examples/ns3-dev-li-ion-energy-source-debug', 'build/src/fd-net-device/examples/ns3-dev-dummy-network-debug', 'build/src/fd-net-device/examples/ns3-dev-fd2fd-onoff-debug', 'build/src/fd-net-device/examples/ns3-dev-realtime-dummy-network-debug', 'build/src/fd-net-device/examples/ns3-dev-realtime-fd2fd-onoff-debug', 'build/src/fd-net-device/examples/ns3-dev-fd-emu-ping-debug', 'build/src/fd-net-device/examples/ns3-dev-fd-emu-udp-echo-debug', 'build/src/fd-net-device/examples/ns3-dev-fd-emu-onoff-debug', 'build/src/fd-net-device/examples/ns3-dev-fd-tap-ping-debug', 'build/src/fd-net-device/examples/ns3-dev-fd-tap-ping6-debug', 'build/src/internet/examples/ns3-dev-main-simple-debug', 'build/src/lte/examples/ns3-dev-lena-cqi-threshold-debug', 'build/src/lte/examples/ns3-dev-lena-dual-stripe-debug', 'build/src/lte/examples/ns3-dev-lena-fading-debug', 'build/src/lte/examples/ns3-dev-lena-intercell-interference-debug', 'build/src/lte/examples/ns3-dev-lena-pathloss-traces-debug', 'build/src/lte/examples/ns3-dev-lena-profiling-debug', 'build/src/lte/examples/ns3-dev-lena-rem-debug', 'build/src/lte/examples/ns3-dev-lena-rem-sector-antenna-debug', 'build/src/lte/examples/ns3-dev-lena-rlc-traces-debug', 'build/src/lte/examples/ns3-dev-lena-simple-debug', 'build/src/lte/examples/ns3-dev-lena-simple-epc-debug', 'build/src/lte/examples/ns3-dev-lena-x2-handover-debug', 'build/src/lte/examples/ns3-dev-lena-x2-handover-measures-debug', 'build/src/mesh/examples/ns3-dev-mesh-debug', 'build/src/mobility/examples/ns3-dev-main-grid-topology-debug', 'build/src/mobility/examples/ns3-dev-main-random-topology-debug', 'build/src/mobility/examples/ns3-dev-main-random-walk-debug', 'build/src/mobility/examples/ns3-dev-mobility-trace-example-debug', 'build/src/mobility/examples/ns3-dev-ns2-mobility-trace-debug', 'build/src/mobility/examples/ns3-dev-bonnmotion-ns2-example-debug', 'build/src/mpi/examples/ns3-dev-simple-distributed-debug', 'build/src/mpi/examples/ns3-dev-third-distributed-debug', 'build/src/mpi/examples/ns3-dev-nms-p2p-nix-distributed-debug', 'build/src/netanim/examples/ns3-dev-dumbbell-animation-debug', 'build/src/netanim/examples/ns3-dev-grid-animation-debug', 'build/src/netanim/examples/ns3-dev-star-animation-debug', 'build/src/netanim/examples/ns3-dev-wireless-animation-debug', 'build/src/netanim/examples/ns3-dev-uan-animation-debug', 'build/src/netanim/examples/ns3-dev-dynamic_linknode-debug', 'build/src/network/examples/ns3-dev-main-packet-header-debug', 'build/src/network/examples/ns3-dev-main-packet-tag-debug', 'build/src/network/examples/ns3-dev-red-tests-debug', 'build/src/network/examples/ns3-dev-droptail_vs_red-debug', 'build/src/nix-vector-routing/examples/ns3-dev-nix-simple-debug', 'build/src/nix-vector-routing/examples/ns3-dev-nms-p2p-nix-debug', 'build/src/olsr/examples/ns3-dev-simple-point-to-point-olsr-debug', 'build/src/olsr/examples/ns3-dev-olsr-hna-debug', 'build/src/point-to-point/examples/ns3-dev-main-attribute-value-debug', 'build/src/propagation/examples/ns3-dev-main-propagation-loss-debug', 'build/src/propagation/examples/ns3-dev-jakes-propagation-model-example-debug', 'build/src/spectrum/examples/ns3-dev-adhoc-aloha-ideal-phy-debug', 'build/src/spectrum/examples/ns3-dev-adhoc-aloha-ideal-phy-matrix-propagation-loss-model-debug', 'build/src/spectrum/examples/ns3-dev-adhoc-aloha-ideal-phy-with-microwave-oven-debug', 'build/src/tap-bridge/examples/ns3-dev-tap-csma-debug', 'build/src/tap-bridge/examples/ns3-dev-tap-csma-virtual-machine-debug', 'build/src/tap-bridge/examples/ns3-dev-tap-wifi-virtual-machine-debug', 'build/src/tap-bridge/examples/ns3-dev-tap-wifi-dumbbell-debug', 'build/src/tools/examples/ns3-dev-gnuplot-example-debug', 'build/src/topology-read/examples/ns3-dev-topology-read-debug', 'build/src/uan/examples/ns3-dev-uan-cw-example-debug', 'build/src/uan/examples/ns3-dev-uan-rc-example-debug', 'build/src/virtual-net-device/examples/ns3-dev-virtual-net-device-debug', 'build/src/wifi/examples/ns3-dev-wifi-phy-test-debug', 'build/src/wimax/examples/ns3-dev-wimax-ipv4-debug', 'build/src/wimax/examples/ns3-dev-wimax-multicast-debug', 'build/src/wimax/examples/ns3-dev-wimax-simple-debug', 'build/examples/wireless/ns3-dev-mixed-wireless-debug', 'build/examples/wireless/ns3-dev-wifi-adhoc-debug', 'build/examples/wireless/ns3-dev-wifi-clear-channel-cmu-debug', 'build/examples/wireless/ns3-dev-wifi-ap-debug', 'build/examples/wireless/ns3-dev-wifi-wired-bridging-debug', 'build/examples/wireless/ns3-dev-simple-wifi-frame-aggregation-debug', 'build/examples/wireless/ns3-dev-multirate-debug', 'build/examples/wireless/ns3-dev-wifi-simple-adhoc-debug', 'build/examples/wireless/ns3-dev-wifi-simple-adhoc-grid-debug', 'build/examples/wireless/ns3-dev-wifi-simple-infra-debug', 'build/examples/wireless/ns3-dev-wifi-simple-interference-debug', 'build/examples/wireless/ns3-dev-wifi-blockack-debug', 'build/examples/wireless/ns3-dev-ofdm-validation-debug', 'build/examples/wireless/ns3-dev-wifi-hidden-terminal-debug', 'build/examples/realtime/ns3-dev-realtime-udp-echo-debug', 'build/examples/udp-client-server/ns3-dev-udp-client-server-debug', 'build/examples/udp-client-server/ns3-dev-udp-trace-client-server-debug', 'build/examples/tcp/ns3-dev-tcp-large-transfer-debug', 'build/examples/tcp/ns3-dev-tcp-nsc-lfn-debug', 'build/examples/tcp/ns3-dev-tcp-nsc-zoo-debug', 'build/examples/tcp/ns3-dev-tcp-star-server-debug', 'build/examples/tcp/ns3-dev-star-debug', 'build/examples/tcp/ns3-dev-tcp-bulk-send-debug', 'build/examples/tcp/ns3-dev-tcp-nsc-comparison-debug', 'build/examples/tcp/ns3-dev-tcp-variants-comparison-debug', 'build/examples/udp/ns3-dev-udp-echo-debug', 'build/examples/error-model/ns3-dev-simple-error-model-debug', 'build/examples/socket/ns3-dev-socket-bound-static-routing-debug', 'build/examples/socket/ns3-dev-socket-bound-tcp-static-routing-debug', 'build/examples/socket/ns3-dev-socket-options-ipv4-debug', 'build/examples/socket/ns3-dev-socket-options-ipv6-debug', 'build/examples/ipv6/ns3-dev-icmpv6-redirect-debug', 'build/examples/ipv6/ns3-dev-ping6-debug', 'build/examples/ipv6/ns3-dev-radvd-debug', 'build/examples/ipv6/ns3-dev-radvd-two-prefix-debug', 'build/examples/ipv6/ns3-dev-test-ipv6-debug', 'build/examples/ipv6/ns3-dev-fragmentation-ipv6-debug', 'build/examples/ipv6/ns3-dev-loose-routing-ipv6-debug', 'build/examples/energy/ns3-dev-energy-model-example-debug', 'build/examples/naming/ns3-dev-object-names-debug', 'build/examples/tutorial/ns3-dev-hello-simulator-debug', 'build/examples/tutorial/ns3-dev-first-debug', 'build/examples/tutorial/ns3-dev-second-debug', 'build/examples/tutorial/ns3-dev-third-debug', 'build/examples/tutorial/ns3-dev-fourth-debug', 'build/examples/tutorial/ns3-dev-fifth-debug', 'build/examples/tutorial/ns3-dev-sixth-debug', 'build/examples/routing/ns3-dev-dynamic-global-routing-debug', 'build/examples/routing/ns3-dev-static-routing-slash32-debug', 'build/examples/routing/ns3-dev-global-routing-slash32-debug', 'build/examples/routing/ns3-dev-global-injection-slash32-debug', 'build/examples/routing/ns3-dev-simple-global-routing-debug', 'build/examples/routing/ns3-dev-simple-alternate-routing-debug', 'build/examples/routing/ns3-dev-mixed-global-routing-debug', 'build/examples/routing/ns3-dev-simple-routing-ping6-debug', 'build/examples/routing/ns3-dev-manet-routing-compare-debug', 'build/examples/stats/ns3-dev-wifi-example-sim-debug', 'build/examples/matrix-topology/ns3-dev-matrix-topology-debug', 'build/scratch/ns3-dev-emulated_nsc-debug', 'build/scratch/subdir/ns3-dev-subdir-debug', 'build/scratch/ns3-dev-lena-simple-epc-debug', 'build/scratch/ns3-dev-scratch-simulator-debug', 'build/scratch/ns3-dev-lte-debug', 'build/scratch/ns3-dev-emulated-debug', 'build/scratch/ns3-dev-lte-dl-simplest-debug', 'build/scratch/ns3-dev-lte_nsc-debug']

# Scripts that are runnable.
ns3_runnable_scripts = ['csma-bridge.py', 'sample-simulator.py', 'wifi-olsr-flowmon.py', 'tap-csma-virtual-machine.py', 'tap-wifi-virtual-machine.py', 'mixed-wireless.py', 'wifi-ap.py', 'realtime-udp-echo.py', 'first.py', 'simple-routing-ping6.py']

