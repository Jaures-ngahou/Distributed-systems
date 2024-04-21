%% MasterSTEP.cls - Version 1 - Mars 2012 - AF et GM
%% MasterSTEP.cls - Version 1.2 - pxfont + hyperref
%% MasterGGG.cls and MasterGGG.cls - Version 1.5 - new logos (un par mention) 
%% Un Master.cls pour les 2 masters - Version 1.6
%% Un ipgpmaster.cls pour les 2 masters - Version 2 - Mai 2020 - Novembre 2022
\NeedsTeXFormat{LaTeX2e}
\ProvidesPackage{ipgpmaster}

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\ProcessOptions\relax
\LoadClass[11pt,twoside]{article}

\def\mymaster{IPGPUPC}


%---- Spacing around titles
\renewcommand\section{\@startsection {section}{1}{\z@}%
%     {-3.5ex \@plus -1ex \@minus -.2ex}%  before skip
%     {2.3ex \@plus.2ex}% after skip
     {-2.0ex \@plus -1ex \@minus -.2ex}%  before skip
     {1.3ex \@plus.2ex}% after skip
     {\normalfont\Large\bfseries}}
\renewcommand\subsection{\@startsection{subsection}{2}{\z@}%
%     {-3.25ex\@plus -1ex \@minus -.2ex}% 
%     {1.5ex \@plus .2ex}%
     {-2.25ex\@plus -1ex \@minus -.2ex}% 
     {1.0ex \@plus .2ex}%
     {\normalfont\large\bfseries}}
\renewcommand\subsubsection{\@startsection{subsubsection}{3}{\z@}%
%     {-3.25ex\@plus -1ex \@minus -.2ex}% 
%     {1.5ex \@plus .2ex}%
     {-2.0ex\@plus -1ex \@minus -.2ex}% 
     {0.75ex \@plus .2ex}%
     {\normalfont\normalsize\bfseries}}

%---- Table of Contents and sectioning  depth
\setcounter{secnumdepth}{3} 
\setcounter{tocdepth}{2}


\RequirePackage{graphicx} 
\RequirePackage[x11names]{xcolor}

\RequirePackage[pdfstartview=FitH,%
  bookmarks=true,%
  bookmarksopen=true,%
  breaklinks=true,%
  colorlinks=true,%
  linkcolor=blue,anchorcolor=blue,%
  citecolor=blue,filecolor=blue,%
  menucolor=blue,%
  urlcolor=blue]{hyperref}


\RequirePackage{booktabs}  % Prettier tables
\RequirePackage{palatino} % Pretty font
%\RequirePackage{pxfonts,txfonts} % clash with amsmath
\RequirePackage{newtxtext,newtxmath}
\RequirePackage{lineno} % Possibility to add line numbers
\RequirePackage[a4paper,margin=2.5cm]{geometry}
%\RequirePackage[latin1]{inputenc}
\parskip 8pt plus1pt minus1pt
\parindent 0pt

%---- Title page
\def\author{}
\def\title{}
\def\shorttitle{}
\def\spe{}
\def\unit{}
\def\team{}
\def\supervisor{}
\def\keywords{}

\newcounter{firstyear}
\newcounter{lastyear}[firstyear]
\newcounter{thismonth}

\newcommand{\checkyears}{
\setcounter{firstyear}{\the\year}
\setcounter{thismonth}{\the\month}
%\setcounter{thismonth}{7}
\ifnum\the\value{thismonth}<9
   \addtocounter{firstyear}{-1}
\fi
\setcounter{lastyear}{\value{firstyear}}
\stepcounter{lastyear}
}


\newcommand{\yearlabel}{Année Universitaire }
\newcommand{\unitlabel}{Unité d'enseignement : }
%\newcommand{\teamslabel}{\'Equipe(s) : }
\newcommand{\suplabel}{Encadreurs : }
\newcommand{\abstractlabel}{Résumé}
\newcommand{\keyslabel}{Mots clés : }
\newcommand{\mydate}{}


\newcommand{\Entete}{

 \IfLanguagePatterns{english}{
    \renewcommand{\yearlabel}{Academic Year }
    \renewcommand{\unitlabel}{Research Unit: }
 %   \renewcommand{\teamslabel}{Team(s): }
    \renewcommand{\suplabel}{Supervisor(s): }
    \renewcommand{\abstractlabel}{Abstract}
    \renewcommand{\keyslabel}{Keywords: }
    }{}


\vspace*{-2cm}

\begin{minipage}{0.58\linewidth}
\yearlabel \thefirstyear\, -- \thelastyear\\
\unitlabel  \unit\\
%\teamslabel \team\\
\suplabel  \supervisor\\
%\today\\
\end{minipage}
\hfill
\begin{minipage}{0.30\linewidth}
\includegraphics[width=\linewidth]{logo_uy1.jpeg}\\[2mm]
\hspace*{\fill}{\sc \spe}\hspace*{1.5mm}\\[1cm]
\end{minipage}

{\parskip 10pt
\begin{center}
\hrule
\vspace*{1mm}
{\huge\bfseries \title}\\[5mm]
\hrule
\vspace*{4mm}
{\Large\bfseries \author}\\[3mm]
\mydate
\end{center}
}}

%---- Headers and footers
\RequirePackage{fancyhdr}
\addtolength{\headheight}{1.6pt} % si 11pt
\lhead[\fancyplain{}{\bfseries\thepage}]{\fancyplain{}{\author}}
\rhead[\fancyplain{}{\nouppercase{\bfseries \shorttitle}}]%
{\fancyplain{}{\bfseries\thepage}}
%\cfoot{\small Année 2011 -- 2012}
\fancyfoot{}
\fancyfoot[LE,RO]{\itshape \yearlabel \thefirstyear\, -- \thelastyear}
%\renewcommand{\footrulewidth}{0.4pt}
\pagestyle{fancyplain}

%---- Abstract and Keywords
\renewenvironment{abstract}
{
\thispagestyle{empty}
\begin{center}
{\bfseries \abstractlabel}
\end{center}
\small
}
{
\par
{\bfseries \keyslabel}  \keywords
\par}

%---- Bibliographies
\RequirePackage{natbib,hyperref,doi}
\bibliographystyle{plainnat}

%---- French
\RequirePackage[english,french]{babel} % Last language, language by default
\RequirePackage{iflang}
\def\frenchcontentsname{Sommaire}

\usepackage[hypcap=true]{caption}



%---- Captions
\long\def\@caption#1[#2]#3{\par\addcontentsline{\csname
  ext@#1\endcsname}{#1}{\protect\numberline{\csname
  the#1\endcsname}{\ignorespaces #2}}\begingroup
    \@parboxrestore
    \normalsize
    \expandafter\let\expandafter\@tempa\csname @make#1caption\endcsname
    \ifx\@tempa\relax\let\@tempa\@makecaption\fi
    \@tempa{\csname fnum@#1\endcsname}{\ignorespaces #3}\par
  \endgroup}
%
% Here is the content of a .sty file containing definitions using the above
% hook.
%
% The following is the same as the \@makecaption in book.sty:
\long\def\@makefigurecaption#1#2{%
 \vskip 10pt 
 \setbox\@tempboxa\hbox{#1 -- {\small #2}}%
 \ifdim \wd\@tempboxa >\hsize #1 -- {\small #2}\par \else
  \hbox to\hsize{\hfil\box\@tempboxa\hfil} %
  \fi
 }
%
% The following is slightly different from the \@makecaption in book.sty:
\long\def\@maketablecaption#1#2{%
 \setbox\@tempboxa\hbox{#1 -- {\small #2}}%
 \ifdim \wd\@tempboxa >\hsize #1 -- {\small #2}\par \else
  \hbox to\hsize{\hfil\box\@tempboxa\hfil} %
  \fi
 \vskip 10pt 
 }
 
%---- Codes
\usepackage{listings}
\lstdefinestyle{mystyle}{
breakatwhitespace=false,
    commentstyle=\color{gray},
    keywordstyle=\color{Green4},
    numberstyle=\footnotesize\color{gray},
    stringstyle=\color{purple},
    identifierstyle=\color{SteelBlue4},
    basicstyle=\ttfamily,
    numbers=left,
    keepspaces=false,    
    numbers=left,    
    showspaces=false,    
    showstringspaces=true,
    }   
\lstset{style=mystyle}

\usepackage{emptypage} % For the last page
