# cosa-fix

This is a wio avr application.

To build this application:
```bash
wio run
```

Example command to upload slave using wio:
```
wio run --target slave --upload --port /dev/ttyUSB0 --clean
```

Example command to upload master using wio:
```
wio run --target master --upload --port /dev/ttyUSB1 --clean
```
