prefix = homework
depend = *.tex *.bib ./src/*
all: $(prefix).pdf
$(prefix).pdf: $(depend)
	- rm *.aux
	- rm *.toc
	- rm *.log
	- rm *.dvi
	- rm *.bbl
	- rm *.blg
	- rm *.out
	xelatex $(prefix).tex 
	bibtex $(prefix)
	xelatex $(prefix).tex 
	xelatex $(prefix).tex 
	evince $(prefix).pdf &
clean:
	- rm *.pdf
