#!/usr/bin/env bash
set -euo pipefail

# ---------------------------
# Colors
# ---------------------------
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[1;33m"
CYAN="\033[0;36m"
BOLD="\033[1m"
RESET="\033[0m"

info()    { echo -e "${CYAN}[INFO]${RESET} $*"; }
success() { echo -e "${GREEN}[SUCCESS]${RESET} $*"; }
warn()    { echo -e "${YELLOW}[WARN]${RESET} $*"; }
error()   { echo -e "${RED}[ERROR]${RESET} $*" >&2; }

# ---------------------------
# Configuration
# ---------------------------
GCC_VERSION="12.1.0"
BINUTILS_VERSION="2.40"
GDB_VERSION="13.2"
TARGET="i386-elf"
PREFIX="$HOME/opt/cross/gcc"
SRC_DIR="$HOME/src"
MAX_CORES=$(nproc)
[ "$MAX_CORES" -gt 8 ] && MAX_CORES=8

# ---------------------------
# URLs
# ---------------------------
GCC_URL="https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz"
BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.xz"
GDB_URL="https://ftp.gnu.org/gnu/gdb/gdb-${GDB_VERSION}.tar.xz"

# ---------------------------
# Dependencies
# ---------------------------
info "Installing dependencies..."
sudo apt-get update
sudo apt-get install -y bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev wget build-essential
success "Dependencies installed."

# ---------------------------
# Directories
# ---------------------------
info "Preparing directories..."
mkdir -p "$SRC_DIR" "$PREFIX"
cd "$SRC_DIR"
success "Directories ready."

# ---------------------------
# Download sources
# ---------------------------
info "Downloading sources..."
for url in "$GCC_URL" "$BINUTILS_URL" "$GDB_URL"; do
    file=$(basename "$url")
    if [ ! -f "$file" ]; then
        info "Fetching $file..."
        wget -q "$url"
        success "$file downloaded."
    else
        warn "$file already exists, skipping download."
    fi
done

# ---------------------------
# Extract sources
# ---------------------------
info "Extracting sources..."
for tar in *.tar.*; do
    dir="${tar%.tar.*}"
    if [ ! -d "$dir" ]; then
        info "Extracting $tar..."
        tar xf "$tar"
        success "$dir extracted."
    else
        warn "$dir already extracted."
    fi
done

# ---------------------------
# Export PATH
# ---------------------------
export PATH="$PREFIX/bin:$PATH"
info "PATH updated to include $PREFIX/bin"

# ---------------------------
# Build function
# ---------------------------
build_component() {
    local name="$1"
    local dir="$2"
    local configure_args="$3"
    local make_target="${4:-all}"
    local install_target="${5:-install}"
    local cleanup="${6:-yes}"

    info "=== Building $name ==="
    mkdir -p "build-$name"
    cd "build-$name"

    if [ ! -f ".configured" ]; then
        info "Configuring $name..."
        eval "../$dir/configure $configure_args"
        touch .configured
        success "$name configured."
    else
        warn "$name already configured."
    fi

    for target in $make_target; do
        if [ ! -f ".built-$target" ]; then
            info "Compiling $name ($target)..."
            make -j"$MAX_CORES" "$target"
            touch ".built-$target"
            success "$name ($target) built."
        else
            warn "$name ($target) already built."
        fi
    done

    for target in $install_target; do
        if [ ! -f ".installed-$target" ]; then
            info "Installing $name ($target)..."
            make "$target"
            touch ".installed-$target"
            success "$name ($target) installed."
        else
            warn "$name ($target) already installed."
        fi
    done

    cd "$SRC_DIR"
    if [ "$cleanup" = "yes" ]; then
        info "Cleaning up $name build directory..."
        rm -rf "build-$name"
        success "$name build directory cleaned."
    fi
    success "$name build completed."
}

# ---------------------------
# Build Binutils
# ---------------------------
build_component "binutils" "binutils-${BINUTILS_VERSION}" \
    "--target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror"

# ---------------------------
# Build GDB
# ---------------------------
build_component "gdb" "gdb-${GDB_VERSION}" \
    "--target=$TARGET --prefix=$PREFIX --disable-werror" \
    "all-gdb" "install-gdb"

# ---------------------------
# Verify PATH
# ---------------------------
if ! which "$TARGET-as" >/dev/null 2>&1; then
    error "$TARGET-as is not in the PATH! Something went wrong with Binutils."
    exit 1
else
    success "$TARGET-as found in PATH."
fi

# ---------------------------
# Build GCC (incremental)
# ---------------------------
build_component "gcc" "gcc-${GCC_VERSION}" \
    "--target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx" \
    "all-gcc all-target-libgcc all-target-libstdc++-v3" \
    "install-gcc install-target-libgcc install-target-libstdc++-v3" \
    "no"   # Keep build directory until all steps complete

# ---------------------------
# Persist PATH across variable shells (replaceable block)
# ---------------------------
RC_FILE=""
if [ -n "${BASH_VERSION:-}" ]; then
    RC_FILE="$HOME/.bashrc"
elif [ -n "${ZSH_VERSION:-}" ]; then
    RC_FILE="$HOME/.zshrc"
elif [ -n "${SHELL:-}" ]; then
    case "$(basename "$SHELL")" in
        bash) RC_FILE="$HOME/.bashrc" ;;
        zsh)  RC_FILE="$HOME/.zshrc" ;;
        fish) RC_FILE="$HOME/.config/fish/config.fish" ;;
        *)    RC_FILE="$HOME/.profile" ;;
    esac
else
    RC_FILE="$HOME/.profile"
fi

# Define block markers
START_MARKER="# >>> cross-compiler PATH setup >>>"
END_MARKER="# <<< cross-compiler PATH setup <<<"

# Define content to insert between markers
BLOCK_CONTENT="$START_MARKER
export PATH=\"$PREFIX/bin:\$PATH\"
$END_MARKER"

# Create RC file if missing
touch "$RC_FILE"

# If the block exists, replace it; otherwise, append it
if grep -q "$START_MARKER" "$RC_FILE"; then
    # Replace existing block
    sed -i "/$START_MARKER/,/$END_MARKER/c\\$BLOCK_CONTENT" "$RC_FILE"
    info "Updated PATH block in $(basename "$RC_FILE")"
else
    # Append block to end of file
    printf "\n%s\n" "$BLOCK_CONTENT" >> "$RC_FILE"
    success "Added PATH block to $(basename "$RC_FILE")"
fi

export PATH="$PREFIX/bin:$PATH"
success "PATH updated for current session."

# ---------------------------
# Create i386-gcc symlink
# ---------------------------
info "Create symbolic link for shorter compile name"
if [ -f "$PREFIX/bin/i386-elf-gcc" ]; then
    ln -sf "$PREFIX/bin/i386-elf-gcc" "$PREFIX/bin/i386-gcc"
    success "Created symbolic link: $PREFIX/bin/i386-gcc → i386-elf-gcc"
else
    error "Could not create symlink — i386-elf-gcc not found!"
fi

# ---------------------------
# Verify GCC
# ---------------------------
info "Verifying GCC installation..."
if "$PREFIX/bin/$TARGET-gcc" --version >/dev/null 2>&1; then
    "$PREFIX/bin/$TARGET-gcc" --version | head -n1
    success "GCC cross-compiler verification successful!"
    echo -e "${BOLD}Run 'source $RC_FILE' or open a new terminal to activate it globally.${RESET}"
else
    error "GCC verification failed! Compiler not found or not functional."
    exit 1
fi
