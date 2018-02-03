# useful_scripts

A collection of scripts that I've written that sometimes come in handy. Not all
of them are actually all that useful, but it's nice to be able to throw them
on GitHub and forget about them until I need them again.

## allsum

This is a cruddy script that generates many (but not actually all) checksums
for a file, a list of files, or all files in a directory. Currently this gets
an MD5, a SHA1, a SHA256, and a SHA512 sum.

## dynamic_motd

This script is intended to be run when a user logs in to a system. It lists
the number of updates available as well as a good deal of information about
the system and the user. Maintenance information can be written to
`/etc/motd_maint` and this script will display that text to the user.

## labprintconfig

`labprintconfig` add the [JMU LabPrint](http://labprint.jmu.edu) printers to a
computer through CUPS. This works on macOS, ArchLinux, and Linux Mint. It may
work on other UNIX-based OSes that have CUPS as well.

## md5gen

Generates MD5 sums for all the zip files in the current directory.

## stu_home

This uses `nemo` to mount my home directory on stu.cs.jmu.edu.

## stu_install_zsh

Builds and install ZSH within the user's home directory. This is intended to be
used on systems where ZSH is not installed. This also sets ZSH to be launched
on login without requiring changes to /etc/shells. Oh My Zsh can optionally be
installed as well

## update_mirrors

On ArchLinux, this will update the mirrors in use based on the user's current
IP address. /etc/pacman.d/currentmirrors must be what /etc/pacman.conf includes
when looking for mirrors.
