I love this clock!

Play a 25hour clock leading me to another world!

Turning 24hour clock into the 25hour ,I can enjoy another hour per day for free 🤣!

# Sowon

![demo](./demo.gif)

## Build

### Ubuntu

```console
$ sudo apt-get install -qq libgl-dev libxcursor-dev libxrandr-dev libxi-dev
$ make
$ ./sowon
```

### Fedora 42+

```console
$ sudo dnf install libXi-devel libXrandr-devel
$ make
$ ./sowon
```

### MacOS

```console
$ make
$ ./sowon
```

### Windows

#### Visual Studio

- Enter the Visual Studio Command Line Development Environment <https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line>. Basically just find `vcvarsall.bat` and run `vcvarsall.bat x64` inside of cmd

```console
> cd path\to\sowon
> build_msvc
```

## Usage

### Modes

- Ascending mode: `./sowon`
- Descending mode: `./sowon <seconds>`
- Clock Mode: `./sowon clock`
- 25-Hour Clock: `./sowon 25hour`

### Flags

- Start in paused state: `./sowon -p <mode>`
- Exit sowon after countdown finished: `./sowon -e`

### Key bindings

| Key | Description |
| --- | --- |
| <kbd>SPACE</kbd> | Toggle pause |
| <kbd>=</kbd> or <kbd>+</kbd> | Zoom in |
| <kbd>-</kbd> | Zoom out |
| <kbd>0</kbd> | Zoom 100% |
| <kbd>F5</kbd> | Restart |
| <kbd>F11</kbd> | Fullscreen |

### Cat Guard (Sway)

Click **CAT GUARD** in the top-right corner to disable every keyboard by its Sway input ID. Click **PAWS OFF!** to restore them. The keyboard is also restored when Sowon exits normally. This feature requires `swaymsg` and `jq`.
