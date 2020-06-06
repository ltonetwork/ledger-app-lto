FROM ubuntu:18.04
LABEL maintainer="Ignacio Iglesias <contacto@ignacio.xyz>"

VOLUME ["/code"]
WORKDIR /code

ENV BOLOS_ENV /opt/bolos-env
ENV BOLOS_SDK /opt/bolos-sdk

RUN apt-get update
RUN apt-get install -y cmake git build-essential curl libc6-i386 libc6-dev-i386 libusb-1.0-0-dev libudev-dev python3.6 python3.6-dev python3-pip

RUN pip3 install ledgerblue

RUN echo "Create install directories" && \
  mkdir ${BOLOS_ENV} ${BOLOS_SDK}

RUN echo "Install custom gcc" && \
  curl -L https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2 --output /tmp/gcc.tar.bz2 && \
  echo "fb31fbdfe08406ece43eef5df623c0b2deb8b53e405e2c878300f7a1f303ee52  /tmp/gcc.tar.bz2" | sha256sum -c && \
  tar -xvf /tmp/gcc.tar.bz2 -C ${BOLOS_ENV} && \
  rm /tmp/gcc.tar.bz2

RUN echo "Install custom clang" && \
  curl -L https://releases.llvm.org/7.0.1/clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-18.04.tar.xz --output /tmp/clang.tar.xz && \
  echo "e74ce06d99ed9ce42898e22d2a966f71ae785bdf4edbded93e628d696858921a  /tmp/clang.tar.xz" | sha256sum -c && \
  tar -xvf /tmp/clang.tar.xz -C ${BOLOS_ENV} && \
  mv ${BOLOS_ENV}/clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-18.04 ${BOLOS_ENV}/clang-arm-fropi && \
  rm /tmp/clang.tar.xz

ENV PATH /opt/bolos-env/clang-arm-fropi/bin:/opt/bolos-env/gcc-arm-none-eabi-8-2018-q4-major/bin:$PATH

RUN echo "Download Ledger Nano S SDK" && \
  git clone --branch og-1.6.0-1 https://github.com/LedgerHQ/nanos-secure-sdk.git ${BOLOS_SDK}

COPY docker-entrypoint.sh /usr/local/bin/
RUN chmod 777 /usr/local/bin/docker-entrypoint.sh \
    && ln -s /usr/local/bin/docker-entrypoint.sh /

ENTRYPOINT ["docker-entrypoint.sh"]
