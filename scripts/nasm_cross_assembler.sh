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
NASM_VERSION="2.16.01"        # adjust version if needed
PREFIX="$HOME/opt/cross/nasm"
SRC_DIR="$HOME/src"
MAX_CORES=$(nproc)
[ "$MAX_CORES" -gt 8 ] && MAX_CORES=8

# ---------------------------
# URLs
# ---------------------------
NASM_URL="https://www.nasm.us/pub/nasm/releasebuilds/$NASM_VERSION/nasm-$NASM_VERSION.tar.gz"

# ---------------------------
# Dependencies
# ---------------------------
info "Installing dependencies..."
sudo apt-get update
sudo apt-get install -y wget build-essential
success "Dependencies installed."

# ---------------------------
# Create directories
# ---------------------------
info "Preparing directories..."
mkdir -p "$SRC_DIR" "$PREFIX"
cd "$SRC_DIR"
success "Directories ready."

# ---------------------------
# Download NASM
# ---------------------------
file=$(basename "$NASM_URL")
if [ ! -f "$file" ]; then
    info "Fetching $file..."
    wget -q "$NASM_URL"
    success "$file downloaded."
else
    warn "$file already exists, skipping download."
fi

# ---------------------------
# Extract NASM
# ---------------------------
dir="nasm-$NASM_VERSION"
if [ ! -d "$dir" ]; then
    info "Extracting $file..."
    tar xf "$file"
    success "$dir extracted."
else
    warn "$dir already extracted."
fi

# ---------------------------
# Build and install NASM
# ---------------------------
info "Building NASM..."
cd "$dir"
./configure --prefix="$PREFIX"
make -j"$MAX_CORES"
make install
success "NASM installed in $PREFIX"

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
BLOCK_CONTENT=$"$START_MARKER
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
# Verify NASM
# ---------------------------
info "Verifying NASM installation..."
if command -v nasm >/dev/null 2>&1; then
    nasm -v
    success "NASM cross-assembler verification successful!"
    echo -e "${BOLD}Run 'source $RC_FILE' or open a new terminal to activate it globally.${RESET}"
else
    error "NASM verification failed! Command not found."
    exit 1
fi
