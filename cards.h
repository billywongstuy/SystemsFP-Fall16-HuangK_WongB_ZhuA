typedef struct card {
  int value;  // 11 is Jack, 12 is Queen, 13 is King, 1 is A
  char suit;  //chars is actually better: D, C, H, S no need to strcmp, == is fine
} card;


//special cases are A and 2
card greaterCard(struct card c1, struct card c2);

