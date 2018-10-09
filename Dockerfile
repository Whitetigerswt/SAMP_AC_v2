FROM i386/ubuntu:14.04

USER root

WORKDIR /

LABEL maintainer="Whitetigerswt <Whitetigerswt@gmail.com>" \
    description="SAMP_AC_v2 Dockerfile"

# Update OS
RUN apt update && sudo apt upgrade -y 

# Get required build files
RUN apt-get install build-essential git g++-4.4 wget -y 

# Clone main repository
RUN git clone https://github.com/Whitetigerswt/SAMP_AC_v2.git

# Install Boost dependencies
RUN wget https://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.bz2
RUN tar --bzip2 -xf ./boost_1_68_0.tar.bz2
RUN cd boost_1_68_0 && ./bootstrap.sh && ./b2

# Copy compiled binaries into the correct folder.
RUN cp ./boost_1_68_0/bin.v2/libs/chrono/build/gcc-4.8/release/link-static/libboost_chrono.a ./SAMP_AC_v2/Shared/libs --force
RUN cp ./boost_1_68_0/bin.v2/libs/date_time/build/gcc-4.8/release/link-static/threading-multili/libboost_date_time.a ./SAMP_AC_v2/Shared/libs --force
RUN cp ./boost_1_68_0/bin.v2/libs/filesystem/build/gcc-4.8/release/link-static/threading-multi/libboost_filesystem.a ./SAMP_AC_v2/Shared/libs --force
RUN cp ./boost_1_68_0/bin.v2/libs/system/build/gcc-4.8/release/link-static/threading-multi/libboost_system.a ./SAMP_AC_v2/Shared/libs --force
RUN cp ./boost_1_68_0/bin.v2/libs/thread/build/gcc-4.8/release/link-static/threadapi-pthread/threading-multi/libboost_thread.a ./SAMP_AC_v2/Shared/libs --force

