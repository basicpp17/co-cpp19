FROM mcr.microsoft.com/devcontainers/base:dev-alpine3.23

RUN \
  apk add --update --no-cache \
    clang21 \
    libc++-dev \
    cmake \
    ninja \
    qbs \
    git \
    gdb \
    linux-headers \
    py3-pip

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
  PATH="/usr/lib/llvm21/bin:${PATH}" \
  CC=/usr/bin/clang-21 \
  CXX=/usr/bin/clang++-21
