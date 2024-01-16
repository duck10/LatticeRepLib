dnl These are the help files for the various commands
dnl each one is a macro LRLWEB_commandnane($1,$2,$3)
dnl $1 is the prefix, usually <font size=-1>
dnl $2 is the line break, usually <br>
dnl $3 is the epilog, usually </font>a
dnl Command:  CmdDelone
dnl -------------------
define([[[LRLWEB_CmdDelone]]],[[[[[[$1]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[compute Selling-reduced primitive cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[Perform Selling/Delone reduction on input cells$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> S<sup>6</sup> form of the reduced cells of input. The ]]]]]]dnl
[[[[[[Delone-reduced$2]]]]]]dnl
[[[[[[cell has only non-acute interaxial ]]]]]]dnl
[[[[[[angles.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdDists
dnl -------------------
define([[[LRLWEB_CmdDists]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[compute NCDist and CS6Dist distances</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[For a group of input cells, distances are reported in 3 different ]]]]]]dnl
[[[[[[calculations:$2]]]]]]dnl
[[[[[[the simple Euclidean distance, the NCDist (G<sup>6</sup>) ]]]]]]dnl
[[[[[[distance, and the CS6Dist$2]]]]]]dnl
[[[[[[(S<sup>6</sup>) distances. Distances ]]]]]]dnl
[[[[[[are calculated in 2 different ways. The first$2]]]]]]dnl
[[[[[[is the rolling ]]]]]]dnl
[[[[[[calculation of pairs in the list. The second is the distance of each ]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[cell from the first input cell.$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> The distances for each input are output on a single line, starting ]]]]]]dnl
[[[[[[with a $2]]]]]]dnl
[[[[[[semicolon so that it will be ignored by succeeding ]]]]]]dnl
[[[[[[programs. The input cells are echoed$2]]]]]]dnl
[[[[[[to the output line as ]]]]]]dnl
[[[[[[primitive and a, b, c &alpha;, &beta;, &gamma;, so that ]]]]]]dnl
[[[[[[they $2]]]]]]dnl
[[[[[[can be used for further input. Each output cell is ]]]]]]dnl
[[[[[[followed$2]]]]]]dnl
[[[[[[by its ordinal number, and the distance calculations ]]]]]]dnl
[[[[[[list the ordinals for the$2]]]]]]dnl
[[[[[[cells used of each ]]]]]]dnl
[[[[[[calculation.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdGen
dnl -------------------
define([[[LRLWEB_CmdGen]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[Generate cells of a particular type or types</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[To Create valid unit cells of various user-chosen types$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Output cells are reported in G<sup>6</sup> form, ]]]]]]dnl
[[[[[[randomly chosen cell$2]]]]]]dnl
[[[[[[parameters for the various selected types. ]]]]]]dnl
[[[[[[Lattice types are shown following $2]]]]]]dnl
[[[[[[the numerical ]]]]]]dnl
[[[[[[parameters.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> (default values in ]]]]]]dnl
[[[[[[parentheses)$2]]]]]]dnl
[[[[[[--- The number of cells of each type to be ]]]]]]dnl
[[[[[[generated (1)$2]]]]]]dnl
[[[[[[--- The type(s) of selected output requested. ]]]]]]dnl
[[[[[[Examples are$2]]]]]]dnl
[[[[[[--- &quot;all&quot; (default) -- produces examples ]]]]]]dnl
[[[[[[of each of the 44 Niggli types $2]]]]]]dnl
[[[[[[(see the International Tables) ]]]]]]dnl
[[[[[[and also each of the 24 Delone types (all)$2]]]]]]dnl
[[[[[[--- a number between ]]]]]]dnl
[[[[[[1 and 44 will generate examples of that Niggli type. For instance,$2]]]]]]dnl
[[[[[[24 ]]]]]]dnl
[[[[[[will create Niggli type 24 (which is hR). {allowed values: 1-44}$2]]]]]]dnl
[[[[[[--- ]]]]]]dnl
[[[[[[&quot;m&quot; generates monoclinic examples {allowed types: c, t, h, ]]]]]]dnl
[[[[[[o, m, a}$2]]]]]]dnl
[[[[[[--- &quot;mC&quot; generates C-centered monoclinic ]]]]]]dnl
[[[[[[examples {mP, mS, mC, hR, hP, tP, tI, cF, cI, cP, aP}$2]]]]]]dnl
[[[[[[--- &quot;A1&quot; ]]]]]]dnl
[[[[[[generates a general triclinic Delone type with no S<sup>6</sup> zero ]]]]]]dnl
[[[[[[scalars.$2]]]]]]dnl
[[[[[[--- &quot;O4&quot; generates the centered orthorhombic ]]]]]]dnl
[[[[[[Delone type with signature {00r sst}.$2]]]]]]dnl
[[[[[[--- &quot;O&quot; will ]]]]]]dnl
[[[[[[generate all orthorhombic Delone types {allowed types: C, T, H, O, M, ]]]]]]dnl
[[[[[[A}$2]]]]]]dnl
[[[[[[--- Unrecognized input will generate all ]]]]]]dnl
[[[[[[types$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdLM
dnl -------------------
define([[[LRLWEB_CmdLM]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[apply Lattice Matching algorithm to listed cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[Taking the first input cells as the &quot;REFERENCE&quot;, the ]]]]]]dnl
[[[[[[alternative cells of$2]]]]]]dnl
[[[[[[each succeeding cell are examined to find a ]]]]]]dnl
[[[[[[closest match to the reference lattice. $2]]]]]]dnl
[[[[[[<p style=margin-left:40px>Andrews, Lawrence C., Herbert ]]]]]]dnl
[[[[[[J. Bernstein, and Nicholas K. Sauter. $2]]]]]]dnl
[[[[[[&quot;Approximating ]]]]]]dnl
[[[[[[lattice similarity.&quot; ]]]]]]dnl
[[[[[[Acta Crystallographica Section A: ]]]]]]dnl
[[[[[[Foundations and Advances 79.5, 480-484 (2023).</p> ]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> a, b, c &alpha;, &beta;, &gamma;, with the ]]]]]]dnl
[[[[[[lattice centering$2]]]]]]dnl
[[[[[[chosen to be the same as that of the reference ]]]]]]dnl
[[[[[[cell.$2]]]]]]dnl
[[[[[[<em><u>Example:</u></em>$2]]]]]]dnl
[[[[[[f 10 10 10 90 90 90$2]]]]]]dnl
[[[[[[p 7 ]]]]]]dnl
[[[[[[7.1 6.9 59 60 61$2]]]]]]dnl
[[[[[[end$2]]]]]]dnl
[[[[[[; Lattice Matching$2]]]]]]dnl
[[[[[[F 10.000 ]]]]]]dnl
[[[[[[10.000 10.000 90.000 90.000 90.000 ( 0 degrees in S6) REFERENCE$2]]]]]]dnl
[[[[[[F ]]]]]]dnl
[[[[[[10.050 9.821 9.832 89.143 92.094 88.781 ( 3.07179 degrees in ]]]]]]dnl
[[[[[[S6)$2]]]]]]dnl
[[[[[[end$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdNiggli
dnl -------------------
define([[[LRLWEB_CmdNiggli]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[compute Niggli-reduced primitive cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[Perform Niggli reduction on input cells. The result has$2]]]]]]dnl
[[[[[[the ]]]]]]dnl
[[[[[[shortest edge lengths of the lattice and is the &quot;most ]]]]]]dnl
[[[[[[orthogonal&quot; unit cell.$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> G<sup>6</sup> ]]]]]]dnl
[[[[[[form of the reduced cells of input$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdPath
dnl -------------------
define([[[LRLWEB_CmdPath]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[compute path between pairs of cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[A series of points is computed between each pair of input cells. ]]]]]]dnl
[[[[[[The$2]]]]]]dnl
[[[[[[computation is done in S<sup>6</sup>. For n input cells, n-1 ]]]]]]dnl
[[[[[[paths will be$2]]]]]]dnl
[[[[[[generated. The individual points are labeled with ]]]]]]dnl
[[[[[[their ordinals and each $2]]]]]]dnl
[[[[[[segment is labeled (after the 6 ]]]]]]dnl
[[[[[[parameters).$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> S<sup>6</sup>$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[The number of points in -each- path ]]]]]]dnl
[[[[[[(20)$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdPerturb
dnl -------------------
define([[[LRLWEB_CmdPerturb]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[compute perturbed versions of input cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[For each input cell, compute slight altered versions$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> unit cell edge lengths and angles$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[The number of perturbations of each input cell (20) and the amount of$2]]]]]]dnl
[[[[[[perturbation in parts-per-thousand (ppt) (1 ppt). The calculation is done in S<sup>6</sup>,$2]]]]]]dnl
[[[[[[and the perturbation is in a direction orthogonal to the S<sup>6</sup> vector..$2]]]]]]dnl
[[[[[[<em><u>Example:</u></em> ]]]]]]dnl
[[[[[[requesting 10 perturbations and 10 ppt (1%) perturbations.$2]]]]]]dnl
[[[[[[f 10 10 10 90 90 90$2]]]]]]dnl
[[[[[[end$2]]]]]]dnl
[[[[[[; Perturb vectors$2]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[F 10 10 10 90 90 90 original input cell$2]]]]]]dnl
[[[[[[F 9.940 9.916 9.884 89.573 89.357 89.565 perturbed $2]]]]]]dnl
[[[[[[F 10.016 10.029 9.960 89.567 90.358 90.109 perturbed $2]]]]]]dnl
[[[[[[F 9.956 9.921 9.929 89.938 89.497 89.452 perturbed $2]]]]]]dnl
[[[[[[F 9.896 9.992 10.053 90.576 89.709 89.376 perturbed $2]]]]]]dnl
[[[[[[F 9.969 9.989 9.984 90.509 89.490 89.668 perturbed $2]]]]]]dnl
[[[[[[F 9.963 9.984 9.904 89.655 89.147 90.336 perturbed $2]]]]]]dnl
[[[[[[F 10.015 9.928 9.962 89.793 90.276 89.394 perturbed $2]]]]]]dnl
[[[[[[F 10.019 9.922 9.884 89.295 89.772 89.926 perturbed $2]]]]]]dnl
[[[[[[F 9.914 9.918 9.909 89.327 89.348 89.820 perturbed $2]]]]]]dnl
[[[[[[F 10.020 9.978 10.119 90.295 90.410 89.960 perturbed $2]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[; CmdPerturb: number of cells to produce = 10 orthogonal perturbation of 10 ]]]]]]dnl
[[[[[[/1000$2]]]]]]dnl
[[[[[[end$2]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdS6Refl
dnl -------------------
define([[[LRLWEB_CmdS6Refl]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[apply S6 reflections to input cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[The 24 permutations of the S<sup>6</sup> cell vectors. Duplicates are ]]]]]]dnl
[[[[[[removed.$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> S<sup>6</sup>$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2]]]]]]dnl
[[[[[[<em><u>Example:</u></em>$2]]]]]]dnl
[[[[[[f ]]]]]]dnl
[[[[[[10 10 10 90 90 90$2]]]]]]dnl
[[[[[[end$2]]]]]]dnl
[[[[[[; S6 reflections$2]]]]]]dnl
[[[[[[S6 0.00000 ]]]]]]dnl
[[[[[[0.00000 0.00000 -100.00000 -100.00000 -100.00000 0$2]]]]]]dnl
[[[[[[S6 0.00000 ]]]]]]dnl
[[[[[[-100.00000 -100.00000 -100.00000 0.00000 0.00000 1$2]]]]]]dnl
[[[[[[S6 -100.00000 ]]]]]]dnl
[[[[[[0.00000 -100.00000 0.00000 -100.00000 0.00000 2$2]]]]]]dnl
[[[[[[S6 -100.00000 ]]]]]]dnl
[[[[[[-100.00000 0.00000 0.00000 0.00000 -100.00000 3$2]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[or ]]]]]]dnl
[[[[[[presenting those as unit cells (not output by the command):$2]]]]]]dnl
[[[[[[P ]]]]]]dnl
[[[[[[10.000 10.000 10.000 90.000 90.000 90.000 $2]]]]]]dnl
[[[[[[P 17.321 10.000 ]]]]]]dnl
[[[[[[10.000 90.000 125.264 125.264 $2]]]]]]dnl
[[[[[[P 10.000 17.321 10.000 125.264 ]]]]]]dnl
[[[[[[90.000 125.264 $2]]]]]]dnl
[[[[[[P 10.000 10.000 17.321 125.264 125.264 90.000 ]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[The values of 17.321 are from the Delone &quot;d&quot; ]]]]]]dnl
[[[[[[vector, d=-(a+b+c)$2]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[But Niggli reduction of those 4 cells ]]]]]]dnl
[[[[[[gives:$2]]]]]]dnl
[[[[[[G6 100.000 100.000 100.000 0.000 0.000 0.000 $2]]]]]]dnl
[[[[[[G6 ]]]]]]dnl
[[[[[[100.000 100.000 100.000 0.000 0.000 0.000 $2]]]]]]dnl
[[[[[[G6 100.000 100.000 ]]]]]]dnl
[[[[[[100.000 0.000 0.000 0.000 $2]]]]]]dnl
[[[[[[G6 100.000 100.000 100.000 0.000 ]]]]]]dnl
[[[[[[0.000 0.000 ]]]]]]dnl
[[[[[[$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl -----------------------------------------------------
dnl Command:  CmdScale
dnl -------------------
define([[[LRLWEB_CmdScale]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[Scale unit cells data to single size</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em>$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Chosen vector type (one of S6, V7, DC7u, RI (root invariant))$2 ]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> Vector type to scale and output(S6)$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdSella
dnl -------------------
define([[[LRLWEB_CmdSella]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[apply Sella algorithm</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[Determine potentially higher symmetry for input cells$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> agreement factors for each of the Bravais lattice ]]]]]]dnl
[[[[[[types$2]]]]]]dnl
[[[[[[that fill well and reduced cell information for the input ]]]]]]dnl
[[[[[[cells. Also the name$2]]]]]]dnl
[[[[[[of the graphical output file.$2]]]]]]dnl
[[[[[[<em><u>Graphical ]]]]]]dnl
[[[[[[output:</u></em> the figure of the hierarchy of Bravais lattice types ]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[with numerical agreement determined by Sella.$2]]]]]]dnl
[[[[[[<p  style=margin-left:40px> Grimmer, ]]]]]]dnl
[[[[[[H., &amp; Nespolo, M. (2006). $2]]]]]]dnl
[[[[[[Geminography: the crystallography ]]]]]]dnl
[[[[[[of twins. Zeitschrift$2]]]]]]dnl
[[[[[[f&uuml;r Kristallographie-Crystalline ]]]]]]dnl
[[[[[[Materials, 221(1), 28-50.)</p> ]]]]]]dnl
[[[[[[ ]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[choices if only some outputs are needed ]]]]]]dnl
[[[[[[(all)$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdToB4
dnl -------------------
define([[[LRLWEB_CmdToB4]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[compute Bravais tetrahedron (B4)</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[computes the edge vectors of the unit cell and their negative sum. ]]]]]]dnl
[[[[[[These 4$2]]]]]]dnl
[[[[[[vectors also define the Bravais tetrahedron Delone, B. ]]]]]]dnl
[[[[[[N., Galiulin, R. V., $2]]]]]]dnl
[[[[[[&amp; Shtogrin, M. I. (1975). On the ]]]]]]dnl
[[[[[[Bravais types of $2]]]]]]dnl
[[[[[[lattices. Journal of Soviet Mathematics, 4(1), ]]]]]]dnl
[[[[[[79-156.$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> echoes the input cell as a, ]]]]]]dnl
[[[[[[b, c, &alpha;, &beta;, &gamma;, $2]]]]]]dnl
[[[[[[followed by the ]]]]]]dnl
[[[[[[vector representations of the unit cell edges and their vector ]]]]]]dnl
[[[[[[sum,$2]]]]]]dnl
[[[[[[and then a listing of their lengths.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdToC3
dnl -------------------
define([[[LRLWEB_CmdToC3]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[compute complex cell presentation (C</strong><strong><sup>3</sup></strong><strong>)</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[presents input cells as C<sup>3</sup>$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> ]]]]]]dnl
[[[[[[the C<sup>3</sup> version of the input (note that lattice centering ]]]]]]dnl
[[[[[[is not removed)$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<em><u>Example:</u></em>$2]]]]]]dnl
[[[[[[; ]]]]]]dnl
[[[[[[To C3$2]]]]]]dnl
[[[[[[g 100 110 120 0 1 2$2]]]]]]dnl
[[[[[[end$2]]]]]]dnl
[[[[[[C3 (0.000,-101.500) ]]]]]]dnl
[[[[[[(0.500,-111.000) (1.000,-120.500) ]]]]]]dnl
[[[[[[$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdToCell
dnl -------------------
define([[[LRLWEB_CmdToCell]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[compute side-angle cells (a, b, c, &alpha;, &beta;, ]]]]]]dnl
[[[[[[&gamma;)</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> converts input ]]]]]]dnl
[[[[[[cell representations to cell parameters$2]]]]]]dnl
[[[[[[(note that lattice centering is not removed)$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> a, b, c, &alpha;, &beta;, &gamma;. If the ]]]]]]dnl
[[[[[[input cell is not in $2]]]]]]dnl
[[[[[[cell parameters already, then primitive is ]]]]]]dnl
[[[[[[assumed.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdToG6
dnl -------------------
define([[[LRLWEB_CmdToG6]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: compute G</strong><strong><sup>6</sup></strong><strong> version of ]]]]]]dnl
[[[[[[cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> convert to G<sup>6</sup> ]]]]]]dnl
[[[[[[representation (note that lattice centering is not removed)$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> G<sup>6</sup>$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdToS6
dnl -------------------
define([[[LRLWEB_CmdToS6]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: compute S<sup>6</sup> version of cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> convert to S<sup>6</sup> ]]]]]]dnl
[[[[[[representation (note that lattice centering is not removed)$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> S<sup>6</sup>$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdToS6L
dnl -------------------
define([[[LRLWEB_CmdToS6L]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[compute linearized S</strong><strong><sup>6</sup></strong><strong> or ]]]]]]dnl
[[[[[[Root Invariant version of cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[Both of the output choices are 6-space vectors of positive ]]]]]]dnl
[[[[[[numbers.$2]]]]]]dnl
[[[[[[The values are derived by first applying Selling/Delone ]]]]]]dnl
[[[[[[reduction so that all of $2]]]]]]dnl
[[[[[[the S<sup>6</sup> values are ]]]]]]dnl
[[[[[[non-positive. Then the square roots of the negative $2]]]]]]dnl
[[[[[[of those ]]]]]]dnl
[[[[[[values are used to create the result (linearized S<sup>6</sup>). ]]]]]]dnl
[[[[[[The$2]]]]]]dnl
[[[[[[root invariant is generated by applying sorting rules of ]]]]]]dnl
[[[[[[Bright, Cooper, and Kurlin. ]]]]]]dnl
[[[[[[<P style=margin-left:40px>Bright, M., Cooper, A. I., &amp; ]]]]]]dnl
[[[[[[Kurlin, V. (2021). $2]]]]]]dnl
[[[[[[Welcome to a continuous world of ]]]]]]dnl
[[[[[[3-dimensional lattices. $2]]]]]]dnl
[[[[[[arXiv preprint arxiv:2109.11538, ]]]]]]dnl
[[[[[[2.</p> ]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> The type followed by the parameters.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> &quotall&quot or blank for output of both ]]]]]]dnl
[[[[[[types, &quot;R&quot; for root invariant, and S for linearized S<sup>6</sup> ]]]]]]dnl
[[[[[[$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdToU
dnl -------------------
define([[[LRLWEB_CmdToU]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: compute unsorted Dirichlet cells (dc7unsrt)</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[convert to the 7 parameter form related to the Dirichlet$2]]]]]]dnl
[[[[[[(also known as Voronoi or Wigner-Seitz) unit cell. (note that ]]]]]]dnl
[[[[[[lattice centering is not removed)$2]]]]]]dnl
[[[[[[Bernstein and Andrews, 2023.$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> DC7u$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdToV7
dnl -------------------
define([[[LRLWEB_CmdToV7]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: compute V<sup>7</sup> version of cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> convert to ]]]]]]dnl
[[[[[[the 7 parameter vector of Andrews, Bernstein, Pelletier, 1980.$2]]]]]]dnl
[[[[[[The ]]]]]]dnl
[[[[[[vector is define in terms of the Niggli reduced cell a,b,c and the ]]]]]]dnl
[[[[[[Niggli reduced$2]]]]]]dnl
[[[[[[reciprocal cell 1/a*, 1/b*, 1/c*, and the cube ]]]]]]dnl
[[[[[[root of the reduced cell volume.$2]]]]]]dnl
[[[[[[ (note that lattice centering is not removed). Note that all 7 ]]]]]]dnl
[[[[[[scalars are in the units of Angstroms.$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> ]]]]]]dnl
[[[[[[V<sup>7</sup>$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  CmdVolume
dnl -------------------
define([[[LRLWEB_CmdVolume]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[compute volumes of listed cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[compute volumes of listed cells$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> ]]]]]]dnl
[[[[[[Lattice centering, then cell parameters (a, b, c, &alpha;, ]]]]]]dnl
[[[[[[&beta;, &gamma;), $2]]]]]]dnl
[[[[[[followed by the volume$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  Dirichlet
dnl -------------------
define([[[LRLWEB_Dirichlet]]],[[[[[[$1]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[Dirichlet</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[draw Dirichlet cells of listed cells$2]]]]]]dnl
[[[[[[The Dirichlet cell (also known as Voronoi domain, Voronoi cell, ]]]]]]dnl
[[[[[[Delaunay domain, or Wigner-Seitz cell) is the 3-D analog of ]]]]]]dnl
[[[[[[Delaunay triangulation. It is the points nearest to a particular ]]]]]]dnl
[[[[[[lattice point, usually the origin, rather than other lattice points.$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> images of Dirichlet cells in multiple orientations $2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[--blockorigin blkorig (0) and --blocksize blksz(10) $2]]]]]]dnl
[[[[[[limit the portion of the listed cells drawn to prevent overload$2]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------
dnl Command:  PlotC3
dnl -------------------
define([[[LRLWEB_PlotC3]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[[PlotC3</strong>]$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Graphical output of ]]]]]]dnl
[[[[[[multiple unit cells in C<sup>3</sup>$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> ]]]]]]dnl
[[[[[[Name of the file containing the plots$2]]]]]]dnl
[[[[[[<em><u>Graphical output:</u></em> ]]]]]]dnl
[[[[[[2-D plots of the 3 complex coordinates in C<sup>3</sup>$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2]]]]]]dnl
[[[[[[The 3 (2-D) coordinates are plotted in a single row. The ]]]]]]dnl
[[[[[[all-negative quadrants $2]]]]]]dnl
[[[[[[are shown. A consequence is that if the ]]]]]]dnl
[[[[[[unit cells have not been Selling/Delone$2]]]]]]dnl
[[[[[[reduced, then points ]]]]]]dnl
[[[[[[will plot outside of that ]]]]]]dnl
[[[[[[quadrant.$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[Warnings are output for invalid inputs.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[LRL_Web Data Inputs:  There are 5 types of input lines.]]]]]]dnl
[[[[[[ Except for 'END', they can be combined in any order.$2]]]]]]dnl
[[[[[[ All these are case-insensitive. If a particular input lattice is invalid, it is rejected<br /> with a message.$2]]]]]]dnl
[[[[[[---  RANDOM: Random (valid) unit cell;$2]]]]]]dnl
[[[[[[---  Crystal lattice input: 'A', 'B', 'C',]]]]]]dnl
[[[[[[ 'P', 'R', 'F', 'I' followed by three axis lengths]]]]]]dnl
[[[[[[ and three angles (in degrees);$2]]]]]]dnl
[[[[[[---  semicolon: lines beginning with a semicolon are treated as comments$2]]]]]]dnl
[[[[[[---  Vector Input: g (or v or g6) for G6 vectors; s (or s6) for S6, Delone/Selling scalars,]]]]]]dnl
[[[[[[ C3 for C3 input (without parentheses<br />or commas, 'C' would be interpreted as a C-centered unit cell),]]]]]]dnl
[[[[[[ u for unsorted Dirichlet 7-cells.$2]]]]]]dnl
[[[[[[---  END: ends the data input section$2]]]]]]dnl
[[[[[[Examples of unit cell inputs$2]]]]]]dnl
[[[[[[P 10 20 30 90 111 90$2]]]]]]dnl
[[[[[[G 100 400 900 0 -215.02 0$2]]]]]]dnl
[[[[[[S6 0 -107.51 0 7.51 -400 -792.49 $2]]]]]]dnl
[[[[[[; this is a comment$2]]]]]]dnl
[[[[[[end$3]]]]]]dnl
dnl
)dnl--------------------------------------------------

