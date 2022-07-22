ARG BUILD_BASE_VERSION=latest
FROM onosproject/oai-build-base:$BUILD_BASE_VERSION AS builder

WORKDIR /root

COPY ./ ./


RUN cd ./cmake_targets \
        && ./build_oai -c -I --eNB --UE -w SIMU -g Release --build-ric-agent --build-ran-slicing \
        && ./nas_sim_tools/build/conf2uedata -c ../openair3/NAS/TOOLS/ue_eurecom_test_sfr.conf -o ran_build/build
