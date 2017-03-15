SRC	:= $(shell egrep -l '^[^%]*\\begin\{document\}' *.tex)
PDF	= $(SRC:%.tex=%.pdf)
TRG	= $(SRC:%.tex=%.dvi)

.PHONY: all
all: $(PDF)

%.pdf: %.tex
	pdflatex --shell-escape $<
	-bibtex $(<:%.tex=%.aux)
	pdflatex --shell-escape $<
	pdflatex --shell-escape $<

.PHONY: clean
clean:
	rm -f *.aux *.bbl *.blg *.log $(PDF) $(TRG:%.dvi=%.aux) $(TRG:%.dvi=%.bbl) $(TRG:%.dvi=%.blg) $(TRG:%.dvi=%.log) $(TRG:%.dvi=%.out) $(TRG:%.dvi=%.idx) $(TRG:%.dvi=%.ilg) $(TRG:%.dvi=%.ind) $(TRG:%.dvi=%.toc) $(TRG:%.dvi=%.d) $(TRG:%.dvi=%.lof) $(TRG:%.dvi=%.lol) $(TRG:%.dvi=%.lot)

.PHONY: acrord
acrord: $(PDF)
	$(eval TMPPDF = LaTeX-$(shell sha1sum $< | awk '{print $$1}').pdf)
	$(MAKE) $(WINTMP)/$(TMPPDF)
	cd $(WINTMP); "$(ACRORD)" $(TMPPDF); true

$(WINTMP)/%.pdf: $(PDF)
	cp -a $< $@
