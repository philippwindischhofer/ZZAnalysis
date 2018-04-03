#!/bin/bash

# generate also png as output (if needed)
#for pdffile in *.pdf ; do
#    convert -verbose -density 500 -resize '800' "${pdffile}" "${pdffile%.*}".png
#done

# combine all plots into a big grid
rm collage.pdf
montage -mode concatenate -label '%f' *.pdf miff:- | convert miff:- -quality 100 -density 300 -resize 3500x3500 collage.pdf