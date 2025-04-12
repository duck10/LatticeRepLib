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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[primitive and H<sup>6</sup>, a, b, c &alpha;, &beta;, &gamma;, so that ]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[Any input lattice descriptions are copied to the output.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> (default values in ]]]]]]dnl
[[[[[[parentheses) (deprecated)$2]]]]]]dnl
[[[[[[--- The number of cells of each type to be ]]]]]]dnl
[[[[[[generated (1)$2]]]]]]dnl
[[[[[[--- The type(s) of selected output requested. See Control Parameters $2]]]]]]dnl
[[[[[[<em><u>Control Parameters</u></em> $2]]]]]]dnl
[[[[[[Control information is case sensitive. $2]]]]]]dnl
[[[[[[<em>COUNT:</em> the number of each chosen type to randomly generate (default: 5) $2]]]]]]dnl
[[[[[[<em>TYPE:</em> $2]]]]]]dnl
[[[[[[Examples are$2]]]]]]dnl
[[[[[[--- &quot;all&quot; (default) -- produces examples ]]]]]]dnl
[[[[[[of each of the 44 Niggli types $2]]]]]]dnl
[[[[[[(see the International Tables) ]]]]]]dnl
[[[[[[and also each of the 24 Delone types (all) $2]]]]]]dnl
[[[[[[--- &quot;<strong>Gruber&quot;</strong> (default&ensp; -- produces the ]]]]]]dnl
[[[[[[the requested number of copies of the ]]]]]]dnl
[[[[[[five nearly-Niggli-reduced ]]]]]]dnl
[[[[[[examples of Gruber, 1973. $2]]]]]]dnl
[[[[[[--- &quot;<strong>random&quot;</strong> (default&ensp; -- produces the ]]]]]]dnl
[[[[[[the requested number of random unit cells. $2]]]]]]dnl
[[[[[[--- a number between]]]]]]dnl
[[[[[[<strong>1</strong> and <strong>44</strong> will generate ]]]]]]dnl
[[[[[[examples of that Niggli type. For instance, $2]]]]]]dnl
[[[[[[24 ]]]]]]dnl
[[[[[[will create Niggli type 24 (which is hR). {allowed values: 1-44}$2]]]]]]dnl
[[[[[[--- ]]]]]]dnl
[[[[[[<strong>&quot;m&quot;</strong> generates monoclinic examples {allowed types: c, t, h, ]]]]]]dnl
[[[[[[o, m, a}$2]]]]]]dnl
[[[[[[--- <strong>&quot;mC&quot;</strong> generates C-centered monoclinic ]]]]]]dnl
[[[[[[examples {mP, mS, mC, hR, hP, tP, tI, cF, cI, cP, aP} $2]]]]]]dnl
[[[[[[--- <strong>&quot;A1&quot;</strong> ]]]]]]dnl
[[[[[[generates a general triclinic Delone type with no S<sup>6</sup> zero ]]]]]]dnl
[[[[[[scalars.$2]]]]]]dnl
[[[[[[--- <strong>&quot;O4&quot;</strong> generates the centered orthorhombic ]]]]]]dnl
[[[[[[Delone type with signature {00r sst}.$2]]]]]]dnl
[[[[[[--- <strong>&quot;O&quot;</strong> will ]]]]]]dnl
[[[[[[generate all orthorhombic Delone types {allowed types: C, T, H, O, M, ]]]]]]dnl
[[[[[[A}$2]]]]]]dnl
[[[[[[--- Unrecognized input will generate all ]]]]]]dnl
[[[[[[types$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
dnl Command:  CmdInverse
dnl -------------------
define([[[LRLWEB_CmdInverse]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[Make Inverse</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em>Generate reciprocal cells$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit ]]]]]]dnl
[[[[[[cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
[[[[[[<em><u>Parameters: </u></em>]]]]]]dnl
[[[[[[	NA$2]]]]]]dnl
[[[[[[Example:$2]]]]]]dnl
[[[[[[f 10 10 10 90 90 90$2]]]]]]dnl
[[[[[[end$2]]]]]]dnl
[[[[[[P 7.0711 7.0711 7.0711 60.0000 60.0000 60.0000]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[<hr />]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[$3]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> (H<sup>6</sup>) a, b, c &alpha;, &beta;, &gamma;, with the ]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
dnl Command:  CmdPrim
dnl -------------------
define([[[LRLWEB_CmdPrim]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[convert input cells to primitive</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em>]]]]]]dnl
[[[[[[For non-primitive input cells, a primitive version is returned.$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> (H<sup>6</sup>) a,	b, c, &alpha;, &beta;, &gamma;$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em>]]]]]]dnl
[[[[[[ NA$2]]]]]]dnl
[[[[[[<em><u>Example:</u></em>$2]]]]]]dnl
[[[[[[C:\\Users\\lca\\Source\\Repos\\duck10\\LatticeRepLib\\x64\\Debug>cmdprim$2]]]]]]dnl
[[[[[[; To Primitive$2]]]]]]dnl
[[[[[[f 10 10 10   90 90 90$2]]]]]]dnl
[[[[[[end$2]]]]]]dnl
[[[[[[P 7.0711 7.0711 7.0711 60.0000 60.0000 60.0000]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
dnl Command:  CmdSauc
dnl -------------------
define([[[LRLWEB_CmdSauc]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[SAUC search for alternate matching  unit cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u> ]]]]]]dnl
[[[[[[</u></em>allows searching a database of known unit cells ]]]]]]dnl 
[[[[[[for similar cells; several search methods are available.</em>$ 2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Web page listing matching entries in ]]]]]]dnl  
[[[[[[PDB and COD with similar cells$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> $2]]]]]]dnl 
[[[[[[<em><u>Control Parameters</u></em> $2]]]]]]dnl
[[[[[[Note: Control commands and data values are case-insensitive.$2]]]]]]dnl
[[[[[[<em>METRIC:</em> the search metric to use for searching (or the chosen space). $2]]]]]]dnl
[[[[[[&emsp; Allowed values are L1, L2, G6, V7, D7, S6, DC7unsrt or DC7u (default=DC7unsrt) $2]]]]]]dnl
[[[[[[<em>DOMAIN:</em> allowed values are NEAREST, SPHERE, and RANGE (default=NEAREST) $2]]]]]]dnl
[[[[[[<em>RANGEA:</em> size of the box in which to search, when RANGE is chosen (default = 1.0) $2]]]]]]dnl
[[[[[[<em>RANGEB:</em> size of the box in which to search, when RANGE is chosen (default = 1.0) $2]]]]]]dnl
[[[[[[<em>RANGEC:</em> size of the box in which to search, when RANGE is chosen (default = 1.0) $2]]]]]]dnl
[[[[[[<em>RANGEALPHA:</em> size of the box in which to search, when RANGE is chosen (default = 1.0) $2]]]]]]dnl
[[[[[[<em>RANGEBETA:</em> size of the box in which to search, when RANGE is chosen (default = 1.0) $2]]]]]]dnl
[[[[[[<em>RANGEGAMMA:</em> size of the box in which to search, when RANGE is chosen (default = 1.0) $2]]]]]]dnl
[[[[[[<em>BLOCKSTART:</em> if multiple svg files are written, specifies the ordinal of the first to write $2]]]]]]dnl
[[[[[[&emsp; (mostly for use on the website, default=0)$2]]]]]]dnl
[[[[[[<em>BLOCKSIZE:</em> the number of files to write, starting from BLOCKSTART(mostly for use on the website) $2]]]]]]dnl
[[[[[[&emsp; (on the web, this is limited to 20) (mostly for use on the website) $2]]]]]]dnl
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
[[[[[[(all) (deprecated)$2 ]]]]]]dnl
[[[[[[<em><u>Control Parameters</u></em>$2 ]]]]]]dnl
[[[[[[Control information is NOT case sensitive, and the control name can be misspelled.$2 ]]]]]]dnl
[[[[[[<em>DOGRAPHICS:</em> allow to not write output SVG files (default: TRUE $2 ]]]]]]dnl
[[[[[[<em>BLOCKSTART:</em> if multiple svg files are written, specifies the ordinal of the first to write $2 ]]]]]]dnl
[[[[[[&emsp; (mostly for use on the website) $2 ]]]]]]dnl
[[[[[[<em>BLOCKSIZE:</em> the number of files to write, starting from BLOCKSTART(mostly for use on the website) $2 ]]]]]]dnl
[[[[[[&emsp; (on the web, this is limited to 20) (mostly for use on the website) $2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> echoes the input cell as (H<sup>6</sup>) a, ]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[compute H<sup>6</sup> side-angle cells (a, b, c, &alpha;, &beta;, ]]]]]]dnl
[[[[[[&gamma;)</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> converts input ]]]]]]dnl
[[[[[[cell representations to cell parameters$2]]]]]]dnl
[[[[[[(note that lattice centering is not removed)$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> H<sup>6</sup>, a, b, c, &alpha;, &beta;, &gamma;. If the ]]]]]]dnl
[[[[[[input cell is not in $2]]]]]]dnl
[[[[[[cell parameters already, then primitive is ]]]]]]dnl
[[[[[[assumed.$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
dnl Command:  CmdToP3
dnl -------------------
define([[[LRLWEB_CmdToP3]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: compute polar coordinate presentation (P<sup>3</sup>)</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em>presents input cells as P<sup>3</sup>$2 ]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> the P<sup>3</sup>  ]]]]]]dnl
[[[[[[version of the input (note that lattice centering is not removed)$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em>  ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<em><u>Example:</u></em>$2 ]]]]]]dnl
[[[[[[;$2]]]]]]dnl
[[[[[[To P3$2]]]]]]dnl
[[[[[[g 100 110 120 0 1 2$2]]]]]]dnl
[[[[[[end$2]]]]]]dnl
[[[[[[P3 ((6.12323e-16, 10), (0.0478714, 10.488), (0.104447, 10.954))	$2]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
dnl Command:  CmdToRI
dnl -------------------
define([[[LRLWEB_CmdToRI]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong><strong>Command: compute RI (Root Invariant) version of cells</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> convert to RI representation (note that lattice centering is not removed)$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> the RI ]]]]]]dnl
[[[[[[version of the input $2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> NA $2]]]]]]dnl
[[[[[[Root invariant (<b>RI</b>) is a lattice representation introduced by ]]]]]]dnl
[[[[[[Bright and Kurlin (<b>BK</b>) in several papers. <b>RI</b> is best ]]]]]]dnl
[[[[[[understood as a reordering of the Selling-reduced scalars (space <b>S</b><sup><b>6</b></sup>, ]]]]]]dnl
[[[[[[s<sub>1</sub>,s<sub>2</sub>, s<sub>3</sub>, s<sub>4</sub>, s<sub>5</sub>,]]]]]]dnl
[[[[[[s<sub>6</sub>). $2]]]]]]dnl
[[[[[[<b>Step 1:</b> Create a new vector <i><b>p</b></i> with six elements ]]]]]]dnl
[[[[[[that the negatives of the Selling-reduced <b>S</b><sup><b>6</b></sup> ]]]]]]dnl
[[[[[[vector; $2]]]]]]dnl
[[[[[[<b>Step 2:</b> ]]]]]]dnl
[[[[[[Revise <i><b>p</b></i> by taking the square root of each element.$2]]]]]]dnl
[[[[[[<b>Step 3: </b>Exchange ]]]]]]dnl
[[[[[[the columns of <i><b>p</b></i> to place the column that has the small ]]]]]]dnl
[[[[[[scalar into the first column.$2]]]]]]dnl
[[[[[[<b>Step 4:</b> If ]]]]]]dnl
[[[[[[the smallest element is in the second row, exchange the first row and ]]]]]]dnl
[[[[[[second row of that column, and do the same for one of the other ]]]]]]dnl
[[[[[[columns.$2]]]]]]dnl
[[[[[[<b>Step 5:</b> Using ]]]]]]dnl
[[[[[[similar logic, place the smallest scalar of the new columns 2 and 3 ]]]]]]dnl
[[[[[[into the second column and in the first row.$2]]]]]]dnl
[[[[[[<b>Step 6:</b> If ]]]]]]dnl 
[[[[[[the first row scalars of columns 1 and 2 are equal, then exchange]]]]]]dnl
[[[[[[those columns such that the smaller second row scalar is in column 1.]]]]]]dnl
[[[[[[There is no need to specify the result for the case where the second]]]]]]dnl
[[[[[[row scalars are equal, because the results would be identical.]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[Above is the logic]]]]]]dnl
[[[[[[for the case described as <b>V</b><sub><b>1</b></sub> by <b>BK</b>,]]]]]]dnl
[[[[[[where <b>V</b><sub><b>1</b></sub>..<b>V</b><sub><b>5</b></sub> refer]]]]]]dnl
[[[[[[to the &ldquo;Voronoi polyhedron&rdquo; types. <b>V</b><sub><b>1</b></sub>]]]]]]dnl
[[[[[[is the most general case, with no 90 degree angles in the reduced]]]]]]dnl
[[[[[[cell. The <b>V</b><sub><b>1</b></sub> polyhedra have 14 sides. <b>BK</b>]]]]]]dnl
[[[[[[also describe separate root invariants for each of the Voronoi types,]]]]]]dnl
[[[[[[where each <b>RI</b> has fewer than 6 scalars.$2]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[Lattice centering, then H<sup>6</sup> cell parameters (a, b, c, &alpha;, ]]]]]]dnl
[[[[[[&beta;, &gamma;), $2]]]]]]dnl
[[[[[[followed by the volume$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em> ]]]]]]dnl
[[[[[[NA$2 ]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
dnl Command:  Follow
dnl -------------------
define([[[LRLWEB_Follow]]],[[[[[[$1]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[Follow</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[This command provides a graphical display of small variations in lattices. In the default mode (POINT),$2]]]]]]dnl
[[[[[[a single cell is read, and its Niggli-reduced cell is calculated.  Multiple cells can be read, and $2]]]]]]dnl
[[[[[[a graphical output is computed for each input cell.  In space S<sup>6</sup>, a straight line is computed $2]]]]]]dnl
[[[[[[between those points. Then the distances from points (default: 100 points&ensp; along that line are $2]]]]]]dnl
[[[[[[calculated to the end point (Niggli-reduced cell, in the POINT case). The graphical output shows those $2]]]]]]dnl
[[[[[[distances for each chosen distance measure (default: NCDist in G<sup>6</sup> and CS6Dist in S<sup>6</sup>). $2]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[For more details on the Follower method, see Andrews and Bernstein:$2]]]]]]dnl
[[[[[[<a href=https://www.cambridge.org/engage/api-gateway/coe/assets/orp/resource/item/63f418f01d2d184063d630a3/original/the-follower-algorithm-and-a-program-using-it-to-explore-spaces.pdf target=_blank>$2]]]]]]dnl
[[[[[[&quot;The Follower algorithm and a program using it to explore spaces&quot;</a>$2]]]]]]dnl 
[[[[[[<em><u>Output type:</u></em>$2]]]]]]dnl
[[[[[[graphical file names and various text$2]]]]]]dnl
[[[[[[<em><u>Graphical output:</u></em>Follow consumes unit cells descriptions.$2]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em>$2]]]]]]dnl
[[[[[[NA<br>$2]]]]]]dnl
[[[[[[<br>$2]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[<em><u>Control Parameters</u></em><br>$2]]]]]]dnl
[[[[[[Control information is NOT case sensitive.<br>$2]]]]]]dnl
[[[[[[^$2]]]]]]dnl
[[[[[[<em>FOLLOWERMODE</em> One of:$2]]]]]]dnl
[[[[[[&emsp;POINT (default)$2]]]]]]dnl
[[[[[[&emsp;LINE$2]]]]]]dnl
[[[[[[&emsp;CHORD$2]]]]]]dnl
[[[[[[&emsp;CHORD3$2]]]]]]dnl
[[[[[[&emsp;TRIANGLE$2]]]]]]dnl
[[[[[[<em>PERTURBATIONS:</em> how many graphs of slightly different starting point to produce (default: 0)$2]]]]]]dnl
[[[[[[<em>PERTURBBY:</em> how much to perturb input cells (default: 0.10)$2]]]]]]dnl
[[[[[[<em>GLITCHTHRESHOLD:</em> how large a change in distance between points to consider are problem (default: 4%)$2]]]]]]dnl
[[[[[[<em>GLITCHSONLY:</em> FALSE (default) causes only files that contain a glitch to be written (default false)$2]]]]]]dnl
[[[[[[<em>DETECTGLITCHS:</em> TRUE causes glitches to be written on the output graphs (default: true)$2]]]]]]dnl
[[[[[[<em>POINTS (=NUMFOLLOWERPOINTS):</em> the number of distance points to plot (default: 100)$2]]]]]]dnl
[[[[[[<em>PRINTDATA (=PRINTDISTANCEDATA):</em> whether to send the input points along the S<sup>6</sup> line to output (default: false)$2]]]]]]dnl
[[[[[[<em>ENABLE:</em> add distance types to the types that will be displayed on output $2]]]]]]dnl
[[[[[[&emsp;(defaults are NC and CS; you can add RI or V7 or DC7U(synonym: U))$2]]]]]]dnl
[[[[[[<em>DISABLE:</em> remove distance types from the list to be shown on the graph$2]]]]]]dnl
[[[[[[<em>SHOWMARKERS (=SHOWDATAMARKERS):</em> show a marker at each data point on the output line (default: TRUE)$2]]]]]]dnl
[[[[[[<em>BLOCKSTART:</em> if multiple svg files are written, specifies the ordinal of the first to write$2]]]]]]dnl
[[[[[[&emsp; (mostly for use on the website)$2]]]]]]dnl
[[[[[[<em>BLOCKSIZE:</em> the number of files to write, starting from BLOCKSTART(mostly for use on the website)$2]]]]]]dnl
[[[[[[&emsp; (on the web, this is limited to 20) (mostly for use on the website)$2]]]]]]dnl
[[[[[[$2]]]]]]dnl
[[[[[[Sample control input:$2]]]]]]dnl
[[[[[[&emsp;Points 77$2]]]]]]dnl
[[[[[[&emsp;Mode Line$2]]]]]]dnl
[[[[[[&emsp;Enable V7$2]]]]]]dnl
[[[[[[<hr>]]]]]]dnl
[[[[[[<strong>Command: Check Input</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> Verify input lattice types and parameters$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
[[[[[[PlotC3</strong>$2]]]]]]dnl
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
[[[[[[<em><u>Output type:</u></em> Valid input is reported as H<sup>6</sup> unit cell a, b, c, &alpha;, &beta;, &gamma;.$2]]]]]]dnl
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
dnl Command:  PlotPolar
dnl -------------------
define([[[LRLWEB_PlotPolar]]],[[[[[[$1 ]]]]]]dnl
[[[[[[<strong>Command: ]]]]]]dnl
[[[[[[PlotPolar</strong>$2]]]]]]dnl
[[[[[[<em><u>Purpose:</u></em> ]]]]]]dnl
[[[[[[Graphical output of ]]]]]]dnl
[[[[[[multiple unit cells in (a,&amp;alpha;), (b,&amp;beta;), (c,&amp;gamma;)$2]]]]]]dnl
[[[[[[<em><u>Output type:</u></em>]]]]]]dnl
[[[[[[Name of the file containing the plots$2]]]]]]dnl
[[[[[[<em><u>Graphical output:</u></em>]]]]]]dnl
[[[[[[2-D plots of the 3 polar coordinates</sup>$2]]]]]]dnl
[[[[[[<em><u>Parameters:</u></em>]]]]]]dnl
[[[[[[NA$2]]]]]]dnl
[[[[[[The 3 (2-D) coordinates are plotted. The]]]]]]dnl
[[[[[[all-positive y half-space is shown for each.$2]]]]]]dnl
[[[[[[$3]]]]]]dnl
dnl
)dnl--------------------------------------------------

