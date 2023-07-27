# Dreamcast Rust/C Interop Template

This repository provides a very simple set up for using Rust code within
an existing C project that can target both the Dreamcast and PC. This works
by including a pre-compiled static Rust library into the final binary.

## Getting Started

### Toolchain
GCC13 added support for the Rust programming language as a front-end. In order to be able to build binaries for the SuperH4
processor, a custom build of the GCC toolchain is required.

#### Build prerequisities

Make sure the following libraries are installed on the host system:

```
sudo apt install gawk patch bzip2 tar make libgmp-dev libmpfr-dev libmpc-dev gettext wget libelf-dev texinfo bison flex sed git build-essential diffutils curl libjpeg-dev libpng-dev python3
```

#### Downloading the source

Create the folder `/opt/toolchains/dc-gcc13` to hold all the source code and resulting binaries. Navigate to the new
folder and checkout the latest version of the KallistiOS repository:

```shell
git clone -b gcc13 https://github.com/KallistiOS/KallistiOS.git kos
```

#### Creating the configuration file

Navigate to `/opt/toolchains/dc-gcc13/kos/utils/dc-chain` and run the following command:

```shell
mv config.mk.bleeding.sample config.mk
```

This will copy the default configuration for compiling GCC13. Next, change the following variables in `config.mk`:

```shell
toolchains_base=/opt/toolchains/dc-gcc13
pass2_languages=c,c++,objc,obj-c++,rust
```

#### Preparing the required libraries and source code

The dc-chain Makefile requires the source for the libraries to be downloaded to specific folders. Multiple scripts are provided
in the repository to assist with setup. Run the following commands to prepare the workspace:

```shell
./download.sh
./unpack.sh
```

Once the sources have been downloaded and unpacked, the GCC12 sources can be safely deleted, if desired. Finally, the latest
GCC13 sources will need to be checked out:

```shell
git clone https://github.com/gcc-mirror/gcc.git gcc-13
```

#### Compiling the toolchain

Edit `config.mk` one last time and change the `sh_gcc_ver` to `13`:

```shell
sh_gcc_ver=13
```

Now, run `make build` inside of the folder and a newly compiled toolchain should exist in
`/opt/toolchains/dc-gcc13/`. Add `/opt/toolchains/dc-gcc13/sh-elf/bin` and `/opt/toolchains/dc-gcc13/arm-eabi/bin` to
your `PATH` variable and everything should be set up!

## Installing cargo-gccrs

In order to use the `cargo` command with GCCRS, a custom plugin has been developed to use with cargo. Using an existing Rust
installation, run the following command to install the plugin:

```shell
cargo install --git https://github.com/spencerelliott/cargo-gccrs.git --branch main cargo-gccrs
```

This modified cargo-gccrs install allows the ability to modify the GCCRS binary used when compiling through
environment variables. Add the following environment variables to your shell to make compilation easier:

```bash
GCCRS_INCOMPLETE_AND_EXPERIMENTAL_COMPILER_DO_NOT_USE=1
GCCRS_CUSTOM_BIN="sh-elf-gccrs"
```

## Compiling the Game Code

Now that you have a Dreamcast-compatible Rust toolchain, you should be able to compile the Rust source in this project
by running a normal CMake build.

### Limitations

* Currently, the CMake script does not detect whether you have modified any of the Rust source so `cargo gccrs build` will have to be called manually or the CMake project will have to be cleaned then compiled again
* When cleaning the CMake project, the Rust project does not get cleaned so you should run `cargo clean` if you plan on switching to a different platform