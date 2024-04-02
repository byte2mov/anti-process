# Anti-Process

This simply stops any process from looking at your program, task manager, process hacker and any application including debuggers.

## How it Works

so we simply place a hook on `NTQUERYSYSTEMINFORMATION` and return false always to crash applications trying to query information about our process.
i also included another way of detecting file start ups that will not allow files to be started via doing the same exact thing to `CreateProccessW` of setting a hook and returning false always.

## Usage

To integrate Anti-Process into your project, simply include the provided code snippets into your existing code.

## Demo

[Watch the Anti-Process Demo](https://file.garden/ZfYtcuFk1Qd5PhyV/showcase_anti_process.mp4)

## Installation

Clone the repository to your local machine:

```bash
git clone https://github.com/byte2mov/anti-process.git
```
