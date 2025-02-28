## Overview

This subnet calculator allows network administrators to:
- Input a network address
- Specify the number of LANs needed
- Define the number of machines required for each LAN
- Automatically calculate optimal subnet configuration

The program calculates and displays the following information for each LAN:
- Number of machines
- Required host bits
- Network address
- Broadcast address
- Prefix notation
- Subnet mask

## Features

- **User-friendly Interface**: Step-by-step prompts guide the user through the required inputs
- **Automatic Subnet Allocation**: Optimally divides available address space based on host requirements
- **Comprehensive Output**: Displays all relevant networking parameters for each subnet
- **Error Handling**: Validates inputs and handles edge cases
- **Efficient Calculations**: Uses bitwise operations for accurate subnet calculations

## Installation

### Prerequisites
- GCC compiler
- Standard C libraries

### Compilation
```bash
gcc -o -lm subnet_calculator subnet_calculator.c
```

### Running the Program
```bash
./subnet_calculator
```

## Usage Example

```
$ ./subnet_calculator

Enter Network address: 192.168.1.1
How many LANs do you have: 2
Enter Machine number for 0th LAN: 99
Enter Machine number for 1th LAN: 3
|||||||||||||OUTPUT|||||||||||||||
LAN = 0:
Number of machines = 99
Host bits = 7
Network Address = 192.168.1.0
Broadcast Address = 192.168.1.127
Prefix = /25
Subnet Mask = 255.255.255.128
||||||||||||||||||||||||||||||||||
LAN = 1:
Number of machines = 3
Host bits = 2
Network Address = 192.168.1.128
Broadcast Address = 192.168.1.131
Prefix = /30
Subnet Mask = 255.255.255.252
||||||||||||||||||||||||||||||||||
```

## Technical Details

### Algorithm
0. Honestly I followed what my Networking teacher class explain and use it as an algorithm.
1. Parse the input network address
2. Determine the number of host bits required for each LAN based on the machine count
3. Calculate the appropriate subnet mask for each LAN
4. Allocate network addresses sequentially to each LAN
5. Calculate broadcast addresses based on network address and subnet mask
6. Generate prefix notation and subnet mask in decimal format


## Limitations

- Supports only IPv4 addresses
- Does not support CIDR notation input (only outputs it)
- Assumes classless inter-domain routing (CIDR)
- Network address input is assumed to be valid

## Future Improvements

- Add support for IPv6 addresses
- Allow CIDR notation input
- Implement VLSM (Variable Length Subnet Masking)
- Add GUI interface
- Export subnet plan to CSV or JSON

## Acknowledgments

- This program was created as an educational tool for understanding subnet calculations or just to verify your manual calculation.
  
![mr robot GIF](https://github.com/user-attachments/assets/a462e792-3cf9-4ccb-8d19-e1832b85b24c)
