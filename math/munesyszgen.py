#!/bin/python 

cols = 80
rows = 26
colpix = 14

w = 1024*cols;
h = 768*rows;

print(f"%%MediaBox 0 0 {w} {h}")

map_pam_sz = w * h * 4
pam_hdr_sz = 73
print(f"map bytes = {map_pam_sz} + p7hdr {pam_hdr_sz} bytes")

print(f"total bytes = {map_pam_sz + pam_hdr_sz} (this many)")
print(f"kby te  u")

print(f"mutool draw -o /demo/SSYSIMAP.pam -F pam /demo/lush/doc/pdf/munemap.pdf")
