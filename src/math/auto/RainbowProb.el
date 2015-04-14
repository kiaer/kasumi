(TeX-add-style-hook
 "RainbowProb"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("report" "a4paper")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("inputenc" "utf8")))
   (TeX-run-style-hooks
    "latex2e"
    "rainbowtab"
    "report"
    "rep10"
    "inputenc"
    "fancyhdr"
    "extramarks"
    "graphicx"
    "lipsum"
    "amsmath"
    "listings"
    "natbib"
    "url"
    "wrapfig")))

