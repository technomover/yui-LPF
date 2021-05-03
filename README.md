# yui-LPF
a Low Pass Filter for KORG NTS-1

yui-LPF is a 4th order Low Pass Filter (-24dB/oct) for [KORG Nu:tekt NTS-1](https://www.korg.com/products/dj/nts_1).
  - an Effect Unit in MODFX slot, made with [logue-SDK](https://github.com/korginc/logue-sdk)
  - knob A: cutoff freq
  - knob B: resonance

It may run on KORG prologue or minilogue xd but is not tested.


# Binary Release (in progress)

Download yui-LPF.zip and unzip.

Transfer yui-LPF.ntkdigunit to your NTS-1 with "NTS-1 digital Librarian"
  + SEE: https://www.korg.com/products/dj/nts_1/librarian_contents.php


# Source and Build Procedure
  + NOTE: the example below is on macOS but others may be similar.

## install logue-SDK
```bash:install logue-SDK
 [somewhere]$ git clone https://github.com/korginc/logue-sdk.git
 [somewhere]$ cd logue-sdk
 [somewhere/logue-sdk]$ git submodule update --init
```
  + SEE: https://korginc.github.io/logue-sdk/
  + SEE: https://korginc.github.io/logue-sdk/ja/

## install GCC toolchain for logue-SDK
```bash:install GCC toolchain for logue-SDK
 [somewhere/logue-sdk]$ cd tools/gcc
 [somewhere/logue-sdk/tools/gcc]$ less README.md
 [somewhere/logue-sdk/tools/gcc]$ uname -s
Darwin
 [somewhere/logue-sdk/tools/gcc]$ ./get_gcc_osx.sh
```
  + SEE: https://github.com/korginc/logue-sdk/tree/master/tools/gcc

## install GNU make
```bash:install GNU make
 [somewhere/logue-sdk/tools/gcc]$ cd ../make
 [somewhere/logue-sdk/tools/make]$ less README.md
 [somewhere/logue-sdk/tools/make]$ uname -s
Darwin
 [somewhere/logue-sdk/tools/make]$ which make
/usr/bin/make    // you already have it.
```
  + SEE: https://github.com/korginc/logue-sdk/tree/master/tools/make

## install ZIP
```bash:install ZIP
 [somewhere/logue-sdk/tools/make]$ cd ../zip
 [somewhere/logue-sdk/tools/zip]$ less README.md
 [somewhere/logue-sdk/tools/zip]$ uname -s
Darwin
 [somewhere/logue-sdk/tools/zip]$ which zip
/usr/bin/zip    // you already have it.
```
  + SEE: https://github.com/korginc/logue-sdk/tree/master/tools/zip

## install logue-cli
```bash:install logue-cli
 [somewhere/logue-sdk/tools/zip]$ cd ../logue-cli
 [somewhere/logue-sdk/tools/logue-cli]$ less README.md
 [somewhere/logue-sdk/tools/logue-cli]$ uname -s
Darwin
 [somewhere/logue-sdk/tools/logue-cli]$ ./get_logue_cli_osx.sh
 [somewhere/logue-sdk/tools/logue-cli]$ ls
README.md			get_logue_cli_msys64.sh*
get_logue_cli_linux.sh*		get_logue_cli_osx.sh*
get_logue_cli_msys.sh*		logue-cli-osx-0.07-2b/
 [somewhere/logue-sdk/tools/logue-cli]$ ls logue-cli-osx-0.07-2b
EULA.md*	logue-cli*
 [somewhere/logue-sdk/tools/logue-cli]$ ln -s logue-cli-osx-0.07-2b/logue-cli .
 [somewhere/logue-sdk/tools/logue-cli]$ ls -l logue-cli
lrwxr-xr-x  1 user  staff  31  5  1 09:38 logue-cli@ -> logue-cli-osx-0.07-2b/logue-cli
 [somewhere/logue-sdk/tools/logue-cli]$ cd ../../..
```
  + SEE: https://github.com/korginc/logue-sdk/tree/master/tools/logue-cli

## git clone [yui-LPF] and build binary
```bash:clone [yui-LPF] repository
 [somewhere]$ git clone https://github.com/technomover/yui-LPF.git
 [somewhere]$ ls
logue-sdk/	yui-LPF/
 [somewhere]$ cd yui-LPF/modfx/projects/yui-LPF
 [somewhere/yui-LPF/modfx/projects/yui-LPF]$ ./build.sh
 [somewhere/yui-LPF/modfx/projects/yui-LPF]$ ls yui-LPF.*unit
yui-LPF.mnlgxdunit	yui-LPF.ntkdigunit	yui-LPF.prlgunit
```

## CONNECT your NTS-1 to transfer
```bash:transfer the unit file to NTS-1
 [somewhere/yui-LPF/modfx/projects/yui-LPF]$ ./trans-ntk.sh
```

## misc.

### Edit yui-LPF.cpp if you want customize the filter.
#### example:
        you can turn it to a -12db/oct or -36db/oct LPF with a few line of modification.
```c++:yui-LPF.cpp
#define NumCas 2  // Number of Filter Cascade: 2 (4-pole)
static dsp::BiQuad yui_Filter	// define our BiQuad instances
[NumCh][NumCas] = {		// TODO: shoud able to change NumCh and NumCas
  { dsp::BiQuad(), dsp::BiQuad() },
  { dsp::BiQuad(), dsp::BiQuad() }
};
```

### BUILD and (if succeed it) TRANS for rapid development and debug (on B-shell)
```bash:BUILD and TRANS
 [somewhere/yui-LPF/modfx/projects/yui-LPF]$ ./build.sh && ./trans-ntk.sh
```

### Acknowledgement
- [kaz saita / kinoshita-lab](https://github.com/kinoshita-lab)
  - thanks to his lecture at [NI-SUI](https://ni-sui.electribe.jp/)
  - and his many other great works
- [centrevillage](https://github.com/centrevillage)
  - thanks to his lecture at NI-SUI
  - this project uses [logue-sdk-template](https://github.com/centrevillage/logue-sdk-template)
