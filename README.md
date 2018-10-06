# swapflush - flush data in swap devices back into memory

## Synopsis

```
swapflush -a | file ...
```

## Description

The swapflush utility removes, and re-adds the specified swap devices
from the system, while ensuring they always remains open to ensure
detach-on-last-close devices such as geli(8) onetime providers survive
the process.

