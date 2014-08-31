SimpleUDPFlooder
================

An attempt to learn about the basics of UDP flooding through C Socket Programming. 

What are we using?
==================

Libnet 1.2 
Documentation is at http://fossies.org/dox/libnet-libnet-1.2/

Installing Libnet API
=====================

On Debian, I installed Libnet through
```
sudo apt-get install libnet1-dev
```

Basic Usage
===========

The makefile will compile a executable called "sup" 
```
Usage: sup <Destination IP> <Destination UDP port> <Number of packets to create>
```
Root priviledges are required to open a raw socket. 
