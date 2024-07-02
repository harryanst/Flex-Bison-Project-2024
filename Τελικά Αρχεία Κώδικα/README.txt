Flex-Bison-Project-2024 

Description 📃
Building a compiler for an imaginary programming language using Flex lexical analyzer and Bison
parser generator for CFLs



!!! Σχόλιο !!!
Το αρχείο parser.y υλοποιεί τον parser χωρίς Error Recovery
Το αρχείο parser_rec.y υλοποιεί τον parser με Error Recovery (υλοποίηση Ερωτήματος 4)  




Execution Instructions ⚙  

Για να τρέξει το πρόγραμμα γράφουμε ./program (προαιρετικό input). 
Αν δε διευκρινήσουμε συγκεκριμμένο input file, το πρόγραμμα θα ψάξει να διαβάσει από default ένα file 
με όνομα file.txt

Τα ονόματα των δύο αρχείων είναι lexer.l και parser.y και τα φτιαχνουμε με "make" αφου κατεβάσουμε το Makefile.

Με "make clean" σβήνουμε τα αρχεία που προέκυψαν από το "make" και τα parser.y, lexer.l
