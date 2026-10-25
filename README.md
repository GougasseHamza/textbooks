# Textbooks

Textbook-style chapters written from my course lecture slides, one folder per subject.
Files are numbered in lecture order. `00_All_Chapters.pdf`, where present, is the combined book.

## Compilers
| File | Pages |
|---|---|
| `compilers/01_Introduction_to_Compilers.pdf` | 13 |
| **`compilers/02-04_Lexical_Analysis_Complete.pdf`** (Lectures 2, 3 and 4 in one read: every automata method with a worked example, plus one end-to-end lexer example; start here) | 24 |
| `compilers/02_Tokens_and_Regular_Expressions.pdf` | 24 |
| `compilers/03_Automating_Lexical_Analysis.pdf` | 21 |
| `compilers/03b_Companion_Lexer_Generation.pdf` (what the automata are for) | 9 |
| `compilers/04_Lexical_Analysis_in_Practice.pdf` (NFA/DFA scanning, catch-all, language quirks, hand-written C scanner, flex) | 22 |
| `compilers/05_Parsers_Top_Down_FIRST_FOLLOW_LL1.pdf` (parsers lecture, rebuilt in concept order: grammars and notation, top-down vs bottom-up, recursive descent and the two questions it asks, FIRST and FOLLOW, LL(1) tables and parsing, then left recursion, left factoring and ambiguity) | 22 |

Problem sets and solutions, in `compilers/problem-sets/`:
`Mock_Midterm_Exam.pdf` + `Mock_Midterm_Solutions.pdf` (the 100-point mock: 5 problems on lexical analysis, grammars and LL(1) parsing, every answer justified, with a box per problem saying where the method lives in the chapters and a coverage summary; `check_ll1.py` recomputes every FIRST/FOLLOW set, table and stack trace),
`Problem_Set_1_Compilers_and_Computational_Theory.pdf` + `Problem_Set_1_Solutions.pdf`,
`Problem_Set_2_Tokens_and_Regular_Expressions.pdf` + `Problem_Set_2_Solutions.pdf`,
`Problem_Set_3_Automating_Lexical_Analysis.pdf` + `Problem_Set_3_Solutions.pdf`,
`Problem_Set_4_Lexical_Analysis.pdf` (the chapter 4 textbook ends with a map from each problem to the sections that prepare it),
`Lexical_Analysis_Problem_Set.pdf` + `Lexical_Analysis_Problem_Set_Solutions.pdf` (one exercise per automata method of the combined guide, one per lexing concept, 20 true/false),
`First_Follow_Advanced_Practice_Sheet.pdf` (parsers lecture),
`Comprehensive_Problem_Set_Lectures_1-3.pdf` (questions, quizzes and solutions in one file).

## Advanced Data Management
| File | Pages |
|---|---|
| `advanced-data-management/01_DBMS_Internals_and_the_Life_of_a_Query.pdf` | 16 |
| `advanced-data-management/02_File_Organization_and_Storage.pdf` | 15 |
| `advanced-data-management/03_Indexing.pdf` | 15 |
| `advanced-data-management/04_Query_Processing_and_Operator_Algorithms.pdf` | 14 |

Labs and class examples, in `advanced-data-management/labs/` (no official solutions; each textbook chapter has a lab companion section):
`Lab_3_Indexing.pdf`, `Lab_4_Operators_Joins_Sorting_Aggregation.pdf`,
`Chapter_1_Class_Examples.pdf`, `Chapter_4_Class_Examples.pdf`.

## Operations Research
| File | Pages |
|---|---|
| `operations-research/00_All_Chapters.pdf` (Lectures 1 to 5 combined) | 106 |
| `operations-research/01_From_Decisions_to_Models.pdf` | 31 |
| `operations-research/02_Geometry_and_Algebra_of_Linear_Programs.pdf` | 21 |
| `operations-research/03_Solving_Linear_Programs.pdf` | 17 |
| `operations-research/04_Duality_of_Linear_Programs.pdf` | 26 |
| `operations-research/05_Simplex_Tableau_and_Sensitivity_Analysis.pdf` (self-contained: every result proved, including those from earlier chapters) | 22 |
| `operations-research/How_to_Write_a_Dual.pdf` (methodology guide: the sign rules and why) | 6 |
| `operations-research/How_to_Manipulate_Vectors_Indices_and_Matrices.pdf` (methods guide: coordinates, x_B and x_N, products, inverses, inequalities, the five proof moves) | 8 |

Problem sets and solutions, in `operations-research/problem-sets/`:
`Problem_Set_1.pdf` + `Problem_Set_1_Solutions.pdf`,
`Problem_Set_2.pdf` + `Problem_Set_2_Solutions.pdf`,
`Problem_Set_3.pdf` + `Problem_Set_3_Solutions.pdf`,
`Problem_Set_4_Duality_with_Solutions.pdf` (Lecture 4: quick checks, six problems, solutions),
`LP_Problem_Set_with_Solutions.pdf` (classics, tricks and proofs for Lectures 1 to 3),
`Graphs_Set_1.pdf`, `Graphs_Set_2.pdf` (graph models, companions to Chapter 1).

Midterm preparation, in `operations-research/midterm-prep/`: `OR_Midterm_Review_Guide.pdf` (24 pages: the review deck's five-station route, Model / Represent / Solve / Certify / Update, worked on one production example with an "exam checks" box per station; the 18 preparation exercises annotated with what each tests, the fastest route, the verified answer, the traps and a twist with its answer; a two-hour mock midterm with full solutions), the course's `Midterm_Review_Slides.pdf` and `Midterm_Prep_Exercises_and_Solutions.pdf`, and `verify.py` (exact basis-enumeration check of every number in the exercises and the mock).

Algebra and proofs, Lectures 1 to 5: `OR_Theory_Review_and_Proof_Set.pdf` (13 pages: notation, ten algebraic moves, eleven proof patterns, theorem map, then 18 proof problems and 18 true/false statements) + `OR_Proof_Set_Solutions.pdf` (15 pages, full solutions under each problem).

## Machine Learning
| File | Pages |
|---|---|
| `machine-learning/00_All_Chapters.pdf` (Lectures 1 to 5 combined) | 127 |
| `machine-learning/ML_Lectures_1-4_One_Continuous_Read.pdf` (every concept, named formula and course-style derivation in one read, with a notation key and a formula sheet) | 31 |
| `machine-learning/ML_Metrics_One_Sheet.pdf` (TP/FP/FN/TN, every metric with its denominator and the question it answers, which cells you can know in a given situation, worked examples, traps) | 3 |
| `machine-learning/ML_Revision_Pack_Lectures_1-4.pdf` (per chapter: revision sheet, understanding questions, quiz, maths derivations and proofs, all with answers; numerical applications at the end) | 31 |
| `machine-learning/01_What_Does_It_Mean_to_Learn.pdf` | 24 |
| `machine-learning/02_From_Representations_to_Decisions.pdf` | 32 |
| `machine-learning/03_From_Fitting_to_Generalization.pdf` | 22 |
| `machine-learning/04_Discriminative_and_Generative_Classification.pdf` | 27 |
| `machine-learning/05_Learning_by_Combining_Models.pdf` (ensembles: voting, bagging and OOB, trees, random forests, MDI and permutation importance, gradient boosting, Newton leaf values, XGBoost gain, multi-class, stacking, mixture of experts; every formula proved and worked by hand) | 33 |

Exercises and project, in `machine-learning/exercises/`:
`Exercises_Chapter_1.pdf`, `Exercises_Chapter_2.pdf`, `Review_Questions_Lecture_3.pdf`, `Tutorial_1.pdf`, `Tutorial_2.pdf`,
`ML_Solutions_Tutorial1_Exercises_Review.pdf` (detailed solutions to all four, each question directly above its answer, 35 pages),
`ML_Solutions_Tutorial2.pdf` (Tutorial 2, ensembles: all nine problems worked in full, every number computed, each sub-question quoted above its answer, 18 pages),
`ML_Maths_Problem_Sets_Problems_Only.pdf` + `ML_Maths_Problem_Sets_Solutions.pdf` (four computation and derivation sets, 25 problems, Lectures 2 to 4),
`ML_Tutorial1_Extra_Practice_Problems.pdf` + `ML_Tutorial1_Extra_Practice_Solutions.pdf` (the Tutorial 1 skills asked differently: reverse-engineering a decision rule, finding the mistakes in a solution, fitting KNN/LDA/Naive Bayes from raw rows, choosing an operating point and calibrating, five proofs, tool selection, detector evasion; plus twelve true/false with justification),
`ML_Understanding_Proofs_Experiments_Problems.pdf` + `ML_Understanding_Proofs_Experiments_Solutions.pdf` (Part I: eight understanding questions and four proof problems: proper losses, Hoeffding and model selection, LDA/NB as linear classifiers, cost versus prevalence; Part II: six real-world experiments to read, diagnose and design: learning curves, ablation and shortcuts, labels only from alerts, input-dependent costs, subgroup metrics, post-deployment monitoring),
`ML_Understanding_Test_Lectures_1-4.pdf` (chapter-by-chapter understanding test: 73 true/false statements and 40 open questions covering every concept of Lectures 1 to 4, with the full answer key at the end),
`Project_Part_1_Auditing_an_Intrusion_Detection_Dataset.pdf`,
`Project_Part_2_Generative_and_Discriminative_Classification.pdf`.

## Information Security
| File | Pages |
|---|---|
| `information-security/00_All_Chapters.pdf` (Lectures 1 to 5 combined; textbook with diagrams for every mechanism: cookie jar, cross-site tracking, respawning, cookie syncing sequence, RTB, clickjacking, fingerprint pipeline, Feistel/DES/AES rounds, cipher modes, DH, RSA key generation, MAC vs signature, chain of trust; definitions, proofs, worked examples, and in every section a "Go deeper" box plus hard questions with answers; Quiz 1 solved inline) | 55 |
| `information-security/01_Foundations_of_Information_Security.pdf` | 11 |
| `information-security/02_The_Web_and_Stateful_Tracking.pdf` | 17 |
| `information-security/03_Stateless_Tracking_and_Browser_Defences.pdf` | 12 |
| `information-security/04_Symmetric_Cryptography.pdf` | 14 |
| `information-security/05_Public_Key_Cryptography_Integrity_and_Trust.pdf` | 12 |
| `information-security/Information_Security_Go_Deeper_Guide.pdf` (earlier slide-recap companion: Quiz 1 solved question by question, TP takeaways) | 39 |

TPs and quiz, in `information-security/tps/`: `TP1_Clickjacking.pdf`, `TP2_Stateful_Tracking.pdf`, `TP3_Browser_Fingerprinting.pdf`, `TP4_Breaking_the_Classics.pdf`, `TP5_AES.pdf`, `TP6_OpenSSL.pdf`, `Quiz_1_Web_Privacy.pdf`.

## Embedded Systems (M233)
| File | Pages |
|---|---|
| **`embedded-systems/Embedded_Systems_Complete_Guide.pdf`** (Lectures 1-2 and all labs from A to Z: Arduino Uno, C++ refresher for the AVR, digital I/O and pull resistors, debouncing, millis() timing, Serial, finite state machines; 27 exercises with solutions, every lab solved, one-page reference; part K adds the five skills the past midterms grade beyond the lectures: energy budget of a battery node, platform choice, I/O inventory, FSM audit with the level-crossing and alarm case studies, code-reading traps) | 42 |
| **`embedded-systems/Embedded_Systems_Midterms_Solved.pdf`** (the four past midterms, 2022-23 / 2023-24 / 2024-25 / Oct 2026, every exercise answered in full: QCM and true-false with justifications, wiring tables read off the breadboard figures, LED and energy calculations, code traces, bug hunts, FSM tables and state diagrams, complete non-blocking sketches; closes with a one-page sheet of every recurring fact and template) | 29 |

`embedded-systems/kit/midterms/` holds the 18 midterm sketches; `python3 midterms/verify_midterms.py` re-runs them all in the simulator.
`embedded-systems/kit/` is an offline simulator and checker: `python3 check.py ex06` grades a sketch against an
exercise, `python3 arduino_sim.py run x.ino --wave --press 1000:3` shows the pin/Serial timeline, two boards can be
linked over Serial, and `python3 arduino_sim.py avr x.ino` type-checks for the real ATmega328P. See `kit/README.md`.
