FROM mcr.microsoft.com/devcontainers/base:alpine-3.18

RUN \
  apk add --update --no-cache \
    clang16 \
    libc++-dev \
    cmake \
    ninja \
    git \
    gdb \
    linux-headers \
    py3-pip \
  && pip install conan

# install gdb c++ support
RUN \
  git clone --depth 1 https://github.com/koutheir/libcxx-pretty-printers.git /usr/share/libcxx-pretty-printers \
  && echo "set print pretty on" > /home/vscode/.gdbinit \
  && echo "set print object on" > /home/vscode/.gdbinit \
  && echo "# See: https://github.com/koutheir/libcxx-pretty-printers" > /home/vscode/.gdbinit \
  && echo "python" > /home/vscode/.gdbinit \
  && echo "import sys" >>/home/vscode/.gdbinit \
  && echo "sys.path.insert(0, '/usr/share/libcxx-pretty-printers/src')" >>/home/vscode/.gdbinit \
  && echo "from libcxx.v1.printers import register_libcxx_printers" >>/home/vscode/.gdbinit \
  && echo "register_libcxx_printers (None)" >>/home/vscode/.gdbinit \
  && echo "end" >>/home/vscode/.gdbinit

ENV \
  CC=/usr/bin/clang \
  CXX=/usr/bin/clang++
