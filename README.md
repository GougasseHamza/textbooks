# Textbooks

Textbook-style chapters written from my course lecture slides, one folder per subject.
Files are numbered in lecture order. `00_All_Chapters.pdf`, where present, is the combined book.

## Compilers
| File | Pages |
|---|---|
| `compilers/01_Introduction_to_Compilers.pdf` | 13 |
| **`compilers/02-04_Lexical_Analysis_Complete.pdf`** (Lectures 2, 3 and 4 in one read: every automata method with a worked example, plus one end-to-end lexer example, the pumping lemma, and the single-regex `/* */` comment DFA; start here) | 25 |
| `compilers/02_Tokens_and_Regular_Expressions.pdf` | 25 |
| `compilers/03_Automating_Lexical_Analysis.pdf` | 21 |
| `compilers/03b_Companion_Lexer_Generation.pdf` (what the automata are for) | 9 |
| `compilers/04_Lexical_Analysis_in_Practice.pdf` (NFA/DFA scanning, catch-all, language quirks, hand-written C scanner, flex) | 22 |
| `compilers/05_Parsers_Top_Down_FIRST_FOLLOW_LL1.pdf` (parsers lecture, rebuilt in concept order: grammars and notation, top-down vs bottom-up, recursive descent and the two questions it asks, FIRST and FOLLOW, LL(1) tables and parsing, then left recursion, left factoring, epsilon-production elimination, selection sets and ambiguity) | 24 |

Problem sets and solutions, in `compilers/problem-sets/`:
`Exam_Kit_Lectures_1-4.pdf` + `Exam_Kit_Lectures_1-4_Solutions.pdf` (a 100-point practice exam on lectures 1 to 4 only: 20 justified MCQs, 5 pipeline questions, 12 constructions from regexes through Thompson, subset construction, minimisation, state elimination, derivatives, pumping lemma, combined DFA and scan traces, a C lexer bug, two flex specs and the HTML-comment DFA, then 20 true/false; `check_lex.py` rebuilds every automaton and scan trace),
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
| `advanced-data-management/04_Query_Processing_and_Operator_Algorithms.pdf` | 15 |
| `advanced-data-management/05_Query_Optimization.pdf` (logical rewrites, statistics and histograms, selectivity and join-size estimation, the cost model, System-R join ordering, stale statistics and correlated columns, reading EXPLAIN, Lab 5 companion) | 18 |
| `advanced-data-management/06_Buffer_Management.pdf` (memory hierarchy and effective access time, why the DBMS runs its own cache, frames / page table / pin count / dirty bit, the read path, OPT / FIFO / LRU / Clock and PostgreSQL's clock-sweep, Belady's anomaly worked, sequential flooding and ring buffers, steal / force and the write-ahead log, reading `Buffers:` and `pg_buffercache`, hands-on companion, classic problems) | 21 |
| `advanced-data-management/The_Essential_of_Chapter_6.pdf` (chapter 6 cut down to what the two mocks test: effective access time, the `Buffers:` line and "read is not disk", LRU / FIFO / clock-sweep traces, sequential flooding and ring buffers, steal / force and the write-ahead log, sizing `shared_buffers`; a mock-drill box per section) | 8 |
| `advanced-data-management/The_Essential_of_Chapter_1.pdf` (chapter 1 cut down to what the two mocks test: the five subsystems and who owns a symptom, parse / bind / rewrite / optimise / execute, the iterator model and plan trees, the SQL the exam makes you write, OLTP versus OLAP; a mock-drill box per section) | 9 |
| `advanced-data-management/The_Essential_of_Chapter_2.pdf` (pages as the unit of cost, slotted pages and rows per page, heap / sorted / hashed files, rows versus columns, PostgreSQL storage and VACUUM versus VACUUM FULL; a mock-drill box per section) | 9 |
| `advanced-data-management/The_Essential_of_Chapter_3.pdf` (B+ tree levels and the cost of one lookup in milliseconds, composite indexes and the leading column, when an index helps: pages not rows, correlation and bitmap scans, the price of an index and "index everything"; a mock-drill box per section) | 9 |
| `advanced-data-management/The_Essential_of_Chapter_4.pdf` (pipeline breakers, external merge sort and its thresholds, nested-loop / sort-merge / hash joins with the mocks' numbers, Batches and work_mem, which join wins and when each is a disaster, hash or sort aggregation; a mock-drill box per section) | 9 |
| `advanced-data-management/The_Essential_of_Chapter_5.pdf` (selectivity arithmetic, reading estimated against actual and the three estimation failures, the cost model and the index-versus-scan crossover, why estimation is the hardest part, join ordering and the levers PostgreSQL offers instead of hints; a mock-drill box per section) | 9 |
| `advanced-data-management/The_Essential_of_the_Tickets.pdf` (22 discussion tickets across chapters 1-6, the mocks' and the other claims each chapter generates, each answered as claim / mechanism / number / consequence, plus the method for a ticket never seen) | 13 |

Labs and class examples, in `advanced-data-management/labs/` (no official solutions; each textbook chapter has a lab companion section):
`Lab_3_Indexing.pdf`, `Lab_4_Operators_Joins_Sorting_Aggregation.pdf`, `Lab_5_Optimization_Estimates_Statistics_Bad_Plans.pdf`,
`Chapter_1_Class_Examples.pdf`, `Chapter_4_Class_Examples.pdf`.
`All_Labs_Companion.pdf` (13 p): SQL refresher on the labs' own idioms (clause order, FILTER, CASE, casts, `generate_series` generators and the one-draw skew trick), the PostgreSQL instruments (EXPLAIN field by field, SET/RESET, pg_stats, ANALYZE / CREATE STATISTICS, pageinspect, pg_buffercache, Parquet + DuckDB), each lab on one page with expected output and exercise answers, a 12-item drill with solutions mapped to the mock's hands-on questions, and a cheat sheet.

Midterm prep, in `advanced-data-management/midterm-prep/`: `Mock_Midterm_Exam.pdf` (the 100-point mock: 5 MCQs, 6 discussion questions, 7 hands-on questions on the `shopstream` database) + `Mock_Midterm_Solutions.pdf` (every answer worked and justified, a box per problem mapping it to the chapters, a coverage summary, and a short buffer-management primer, now backed by chapter 6). `Mock_Exam_v2.pdf` (the professor's 100-point mock in the midterm's format: 8 quick checks, 3 problem situations, a real plan to annotate, 2 of 3 discussion tickets) + `Mock_Exam_v2_Solutions.pdf` (12 p: every answer in the rubric's shape, letter or number first, formula, computation, one-sentence decision, trap; all three discussion tickets answered; a box per part mapping it to chapters 1-6). `Quiz_Refresher_Chapters_1_3_Questions.pdf` (5 p) + `Quiz_Refresher_Chapters_1_3_Answers.pdf` (6 p): 30 rapid-recall questions, 8 multiple choice with justification, and 6 midterm-style problems on chapters 1-3 (storage estimation, file organisation, B+ tree arithmetic, index design, plan reading, discussion), questions and answer sheet as separate files. `Quiz_Refresher_Chapter_4_Questions.pdf` (4 p) + `Quiz_Refresher_Chapter_4_Answers.pdf` (5 p): 20 recall questions, 8 multiple choice, 5 midterm-style problems on chapter 4 (external sort at scale, the same join computed five ways, reading a hash-join plan with Batches, duplicates and groups, discussion), all numbers new. `Quiz_Refresher_Chapter_5_Questions.pdf` (5 p) + `Quiz_Refresher_Chapter_5_Answers.pdf` (6 p): chapter 5 in Mock Exam v2's exact format on the same `adm_lab` numbers, 8 quick checks, 3 problem situations (index versus scan from the cost model, a correlated-columns mis-estimate carried through a join, three-table join ordering), one real plan with a harmless factor-11 under-estimate to diagnose, and 2 discussion tickets (`random_page_cost` on SSD, optimizer hints); answers in the rubric's shape with a reread map.

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
| `machine-learning/ML_Cheat_Sheet_Chapters_1-5.pdf` (page 1: every symbol of chapters 1-5 in one table with its meaning; pages 2-5: every formula and decision rule of the five chapters, one per line with its meaning beside it, boxed by topic) | 5 |
| `machine-learning/ML_Revision_Pack_Lectures_1-4.pdf` (per chapter: revision sheet, understanding questions, quiz, maths derivations and proofs, all with answers; numerical applications at the end) | 31 |
| `machine-learning/01_What_Does_It_Mean_to_Learn.pdf` | 24 |
| `machine-learning/02_From_Representations_to_Decisions.pdf` | 32 |
| `machine-learning/03_From_Fitting_to_Generalization.pdf` | 22 |
| `machine-learning/04_Discriminative_and_Generative_Classification.pdf` | 27 |
| `machine-learning/05_Learning_by_Combining_Models.pdf` (ensembles: voting, bagging and OOB, trees, random forests, MDI and permutation importance, gradient boosting, Newton leaf values, XGBoost gain, multi-class, stacking, mixture of experts; every formula proved and worked by hand) | 33 |
| `machine-learning/The_Essential_of_ML_Chapter_1.pdf` (chapter 1 cut down to what the professor's questions test: the two routes to a machine ability, what counts as experience, the supervised setup, the four ingredients, generalization and shortcuts; a Tutorial-drill box per section answering the sheet's questions in expert shape) | 10 |
| `machine-learning/The_Essential_of_ML_Chapter_2.pdf` (representation and Bayes error, the Bayes decision with costs and the reject option, the loss decides the point prediction, finite data and kNN geometry, ERM and cross-entropy as MLE with the logistic gradient, evaluation basics; Tutorial-drill boxes) | 11 |
| `machine-learning/The_Essential_of_ML_Chapter_3.pdf` (generalization gap and inductive bias, capacity, bias and variance derived, penalties and early stopping, feature and model selection with the winner's-curse proof, honest evaluation; every one of the 20 review questions mapped to a section; Tutorial-drill boxes) | 12 |
| `machine-learning/The_Essential_of_ML_Chapter_4.pdf` (two routes to the posterior, logistic regression and softmax, Gaussian class models with the LDA derivation and Tutorial 1 Q7 worked, naive Bayes, comparing the routes, imbalance, prior shift and unfamiliar inputs; Tutorial-drill boxes) | 11 |
| `machine-learning/The_Essential_of_ML_Chapter_5.pdf` (why combining helps with the variance formula derived, trees, bagging and OOB, random forests and mtry, MDI and permutation importance, gradient boosting with the Newton leaf and the XGBoost gain, bagging versus boosting, stacking, mixture of experts; every Tutorial 2 part plus a new-numbers variant per drill) | 12 |
| `machine-learning/ML_How_the_Professor_Asks.pdf` (the seven question verbs with answer templates, the sixteen pairs he asks you to distinguish, every computation on the sheets with its answer, every derivation with its first line and key step, where each problem was adapted from, a nine-hour plan) | 8 |

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

Quiz refreshers, in `machine-learning/quizzes/`, each as a questions file and a separate answer sheet, every number new: `Quiz_Refresher_Chapters_1_3_Questions.pdf` (5 p) + `_Answers.pdf` (5 p): 30 rapid-recall questions, 8 multiple choice with justification, 6 exam-style problems (three actions with costs, which number to report, kNN before and after scaling, least squares, interpolation and ridge, capacity selection and the bias-variance budget, fraud-detector metrics and leakage). `Quiz_Refresher_Chapter_4_Questions.pdf` (4 p) + `_Answers.pdf` (4 p): 20 recall, 8 multiple choice, 5 problems (a logistic classifier by hand, one-dimensional LDA versus QDA, a naive Bayes spam filter with and without smoothing, imbalance and prior shift, choosing a route). `Quiz_Refresher_Chapter_5_Questions.pdf` (4 p) + `_Answers.pdf` (4 p): 20 recall, 8 multiple choice, 5 problems (variance of an average, a Gini split with MDI credit, bootstrap and OOB arithmetic and m_try, two rounds of gradient boosting by hand, an XGBoost leaf and split gain).

## Information Security
| File | Pages |
|---|---|
| `information-security/00_All_Chapters.pdf` (Lectures 1 to 5 combined, older format; chapters 4 and 5 have since been rewritten as standalone volumes, see below; textbook with diagrams for every mechanism: cookie jar, cross-site tracking, respawning, cookie syncing sequence, RTB, clickjacking, fingerprint pipeline, Feistel/DES/AES rounds, cipher modes, DH, RSA key generation, MAC vs signature, chain of trust; definitions, proofs, worked examples, and in every section a "Go deeper" box plus hard questions with answers; Quiz 1 solved inline) | 55 |
| `information-security/01_Foundations_of_Information_Security.pdf` | 11 |
| `information-security/02_The_Web_and_Stateful_Tracking.pdf` | 17 |
| `information-security/03_Stateless_Tracking_and_Browser_Defences.pdf` | 12 |
| `information-security/04_Symmetric_Cryptography.pdf` (rewritten 2026-10-26 in the textbook format of the compilers and data volumes: concept-order prose, a diagram and a hand-worked example per mechanism (Vigenere group attack, Feistel, DES round and key schedule, AES state and MixColumns, ECB/CBC/CTR, DH), an Anatomy table per algorithm saying what each part does and what breaks if it is removed, a checklist for custom-cipher exam problems, TP companion, common mistakes, classic problems) | 19 |
| `information-security/05_Public_Key_Cryptography_Integrity_and_Trust.pdf` (rewritten 2026-10-26, same format: RSA from the arithmetic up with the slide and TP numbers, hybrid encryption, hashes and the birthday bound, passwords, MAC vs signature, certificates and PKI, steganography; Anatomy tables for RSA, hashes/MACs/signatures and PKI; OpenSSL Missions 1 and 3 companion) | 14 |
| `information-security/Information_Security_Go_Deeper_Guide.pdf` (earlier slide-recap companion: Quiz 1 solved question by question, TP takeaways) | 39 |

TPs and quiz, in `information-security/tps/`: `TP1_Clickjacking.pdf`, `TP2_Stateful_Tracking.pdf`, `TP3_Browser_Fingerprinting.pdf`, `TP4_Breaking_the_Classics.pdf`, `TP5_AES.pdf`, `TP6_OpenSSL.pdf`, `Quiz_1_Web_Privacy.pdf`.

Quiz refresher, in `information-security/quizzes/`: `Quiz_Refresher_Chapter_2_Questions.pdf` (10 p) + `Quiz_Refresher_Chapter_2_Answers.pdf` (8 p): 24 recall questions, 8 statement sets, 6 exam-style problems with new hosts and cookies (the attachment rule on seven requests, parties and origins, two trackers' logs with and without third-party cookies, reading a respawning experiment, writing a cookie sync, framing the portal), and a Round D of 17 questions written exactly like Quiz 1 (scenario, four boxes, select every correct answer; Set-Cookie lines, HTTP exchanges, a respawning table, and a design challenge with a drawing box), every box justified in the answer sheet.

## Embedded Systems (M233)
| File | Pages |
|---|---|
| **`embedded-systems/Embedded_Systems_Complete_Guide.pdf`** (Lectures 1-2 and all labs from A to Z: Arduino Uno, C++ refresher for the AVR, digital I/O and pull resistors, debouncing, millis() timing, Serial, finite state machines; 27 exercises with solutions, every lab solved, one-page reference; part K adds the five skills the past midterms grade beyond the lectures: energy budget of a battery node, platform choice, I/O inventory, FSM audit with the level-crossing and alarm case studies, code-reading traps) | 42 |
| **`embedded-systems/Embedded_Systems_Midterms_Solved.pdf`** (the four past midterms, 2022-23 / 2023-24 / 2024-25 / Oct 2026, every exercise answered in full: QCM and true-false with justifications, wiring tables read off the breadboard figures, LED and energy calculations, code traces, bug hunts, FSM tables and state diagrams, complete non-blocking sketches; closes with a one-page sheet of every recurring fact and template) | 29 |

`embedded-systems/kit/midterms/` holds the 18 midterm sketches; `python3 midterms/verify_midterms.py` re-runs them all in the simulator.
`embedded-systems/kit/` is an offline simulator and checker: `python3 check.py ex06` grades a sketch against an
exercise, `python3 arduino_sim.py run x.ino --wave --press 1000:3` shows the pin/Serial timeline, two boards can be
linked over Serial, and `python3 arduino_sim.py avr x.ino` type-checks for the real ATmega328P. See `kit/README.md`.
