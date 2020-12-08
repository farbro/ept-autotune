# Entropy Piano Tuner - Flywheel Tuner Mod

## Information about this fork
This fork of Entropy Piano Tuner has experimental support for the [automatic flywheel tuner device](https://neek.se/pianotuner). The goal is to integrate and control the tuning device in order to automatically tune the piano's tuning pins to the frequency that has been calculated by Entropy Piano Tuner.

## Information

For general information about the software have a look at the [homepage](http://piano-tuner.org/) of the project.

## Building

### Quick instructions
The fundamental workflow to complie the Entropy Piano Tuner is the following. For further information regarding your platform have a look in the [developer pages](http://develop.piano-tuner.org).

* Install [Qt](https://www.qt.io/download-open-source/) for your platform.
* Follow the instructions of [qtmidi](https://gitlab.com/tp3/qtmidi) to build the required midi plugin.
* Clone the full repositoy including all submodules via `git clone --recursive https://gitlab.com/tp3/Entropy-Piano-Tuner.git`.
* Open the `entropypianotuner.pro` file using the QtCreator to build and run the project. Alternatively you can use `qmake` to build from the console.


### Developer pages
Regarding development please have a look at the [developer pages](http://develop.piano-tuner.org) for further information.
