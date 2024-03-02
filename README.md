# swapflush - flush data in swap devices back into memory

## Synopsis

```
swapflush -a | file ...
```

## Description

The swapflush utility uses [swapon(8)](https://man.freebsd.org/cgi/man.cgi?query=swapon&sektion=8&manpath=freebsd-release) to remove then re-add the specified swap devices, while ensuring that they always remain open — for detach-on-last-close devices such as [geli(8)](https://man.freebsd.org/cgi/man.cgi?query=geli&sektion=8&manpath=freebsd-release) onetime providers to survive the process.
