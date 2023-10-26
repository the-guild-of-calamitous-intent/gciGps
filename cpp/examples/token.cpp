#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace std;

#define CSTR_LEN 16
#define CVEC_LEN 18

// Fixed string array
struct cstr_t {
  cstr_t() {}
  ~cstr_t() {}

  bool set(const char* s, size_t sz) {
    if (sz >= CSTR_LEN) return false;
    memcpy(data, s, sz);
    data[sz] = '\0';
    size = sz + 1;
    return true;
  }

  void clear() {
    memset(data,'\0',CSTR_LEN);
    size = 0;
  }

  char data[CSTR_LEN]{'\0'};
  size_t size=0;
};

// Fixed size vector of fixed string arrays
struct cvec_t {
  cvec_t () {}
  ~cvec_t () {}

  // bool push_back(const char* s, size_t sz) {
  //   bool ok = array[index].set(s,sz);
  //   if (!ok) return false;
  //   index++;
  //   return true;
  // }

  // void pop() {
  //   if (index > 0) index--;
  //   // return array[index+1];
  // }
  char* get_next() {
    // index++;
    if (index >= CVEC_LEN) return nullptr;
    return array[index++].data;
  }

  inline
  const size_t size() const { return index; }

  inline
  const char* operator[](const size_t i) const {
    if (i >= CVEC_LEN) return nullptr;
    return array[i].data;
  }

  void clear() {
    int i = CVEC_LEN;
    while (i >= 0) array[i--].clear();
    index = 0;
  }

  cstr_t array[CVEC_LEN];
  size_t index{0};
  // const size_t max_size{CVEC_LEN};
};

cvec_t v;

bool parseGPGSA(char const* input) {
  char const* token_start = input;
  char const* token_end = input;
  // int i = 0;
  size_t str_size = 0;

  while (*token_end != '\0') {
    if (*token_end == ',' || *token_end == '*') {
      if (token_start == token_end) { // empty
        // bool ok = v.push_back("#",1); // empty
        // if (ok == false) printf("crap\n");
        char* p = v.get_next();
        p[0] = '#';
        p[1] = '\0';
      }
      else {
        // *token_end = '\0';
        // bool ok = v.push_back(token_start, strlen(token_start));
        // if (ok == false) printf("crap2\n");
        char* p = v.get_next();
        if (p == nullptr) return false;
        strncpy(p, token_start, str_size-1); // str_size-1??
      }
      // reset for next token
      token_start = token_end + 1;
      str_size = 0;
    }

    token_end++;
    str_size++;
  }

  if (token_start != token_end) {
    // bool ok = v.push_back(token_start, strlen(token_start));
    //     if (ok == false) printf("crap3\n");
    char* p = v.get_next();
    if (p == nullptr) return false;
    strncpy(p, token_start, str_size-1); // str_size-1??
  }

  return true;
}

int main() {
  vector<char const*> msgs{
    "$GPGSA,A,3,03,32,02,04,,,,,,,,,2.01,1.77,0.95*08\0",
    "$GPGGA,234805.000,3906.7106,N,12120.3144,W,1,04,1.77,135.6,M,-22.1,M,,*50\0"
  };

  for (char const* input: msgs) {
    parseGPGSA(input);

    for (int i=0; i < CVEC_LEN; ++i) printf("%d: %s ", i, v[i]);
    printf("\nvector size: %zu\n", v.size());
    printf("%s\n", input);
    printf("-----------------------------------\n");

    v.clear();
  }

  return 0;
}