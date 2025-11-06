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
[ "$MAX_CORES" -gt 8 ] && MAX_CORES=8

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

build_component() {
    local name="$1"
    local dir="$2"
    local configure_args="$3"
    local make_target="${4:-all}"
    local install_target="${5:-install}"

    echo "=== Building $name ==="
    mkdir -p "build-$name"
    cd "build-$name"
    eval "../$dir/configure $configure_args"
    make -j"$MAX_CORES" "$make_target"
    make "$install_target"
    cd "$SRC_DIR"
    rm -rf "build-$name"    # Clean up build directory
}

# Build Binutils
build_component "binutils" "binutils-${BINUTILS_VERSION}" "--target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror"

# Build GDB
build_component "gdb" "gdb-${GDB_VERSION}" "--target=$TARGET --prefix=$PREFIX --disable-werror" "all-gdb" "install-gdb"

# Verify binutils in PATH
if ! which "$TARGET-as" >/dev/null 2>&1; then
    echo "$TARGET-as is not in the PATH!"
fi

# Build GCC
build_component "gcc" "gcc-${GCC_VERSION}" "--target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx" "all-gcc all-target-libgcc all-target-libstdc++-v3" "install-gcc install-target-libgcc install-target-libstdc++-v3"

# Verify GCC installation
"$PREFIX/bin/$TARGET-gcc" --version

# Add cross compiler to PATH for current shell
export PATH="$PREFIX/bin:$PATH"
echo "Cross-compiler installed successfully! PATH updated for current session."

