SimpleUDPFlooder
================

An attempt to learn about the basics of UDP flooding through C Socket Programming. 

What are we using?
==================

Libnet 1.2 
Documentation is at http://fossies.org/dox/libnet-libnet-1.2/

Basic Usage
===========

The makefile will compile a executable called "sup" 
Usage: sup <Destination IP> <Destination UDP port> <Number of packets to create>

Root priviledes are required to open a raw socket. 
