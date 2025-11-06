#!/usr/bin/env bash
set -euo pipefail

# Configuration
GCC_VERSION="12.1.0"
BINUTILS_VERSION="2.40"
GDB_VERSION="13.2"
TARGET="i386-elf"
PREFIX="$HOME/opt/cross"
SRC_DIR="$HOME/src"
MAX_CORES=$(nproc)

# URLs
GCC_URL="https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz"
BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.xz"
GDB_URL="https://ftp.gnu.org/gnu/gdb/gdb-${GDB_VERSION}.tar.xz"

# Ensure dependencies
sudo apt-get update
sudo apt-get install -y bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev wget build-essential

# Create directories
mkdir -p "$SRC_DIR" "$PREFIX"
cd "$SRC_DIR"

# Download sources
for url in "$GCC_URL" "$BINUTILS_URL" "$GDB_URL"; do
    file=$(basename "$url")
    if [ ! -f "$file" ]; then
        wget "$url"
    fi
done

# Extract sources
for tar in *.tar.*; do
    dir="${tar%.tar.*}"
    if [ ! -d "$dir" ]; then
        tar xf "$tar"
    fi
done

# Export environment variables
export PATH="$PREFIX/bin:$PATH"

# Build Binutils
mkdir -p build-binutils
cd build-binutils
../binutils-${BINUTILS_VERSION}/configure --target="$TARGET" --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j"$MAX_CORES"
make install
cd "$SRC_DIR"

# Build GDB
mkdir -p build-gdb
cd build-gdb
../gdb-${GDB_VERSION}/configure --target="$TARGET" --prefix="$PREFIX" --disable-werror
make -j"$MAX_CORES" all-gdb
make install-gdb
cd "$SRC_DIR"

# Verify binutils in PATH
if ! which "$TARGET-as" >/dev/null 2>&1; then
    echo "$TARGET-as is not in the PATH!"
fi

# Build GCC
mkdir -p build-gcc
cd build-gcc
../gcc-${GCC_VERSION}/configure \
    --target="$TARGET" \
    --prefix="$PREFIX" \
    --disable-nls \
    --enable-languages=c,c++ \
    --without-headers \
    --disable-hosted-libstdcxx
make -j"$MAX_CORES" all-gcc
make -j"$MAX_CORES" all-target-libgcc
make -j"$MAX_CORES" all-target-libstdc++-v3
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3
cd "$SRC_DIR"

# Verify GCC installation
"$PREFIX/bin/$TARGET-gcc" --version

# Add cross compiler to PATH for current shell
export PATH="$PREFIX/bin:$PATH"
echo "Cross-compiler installed successfully! PATH updated for current session."

