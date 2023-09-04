#pragma once

#ifdef WIN
  #include <vector>
  std::vector<const char*> splitArgs(std::string_view str, const char pattern)
  {
    int posInit = 0;
    int posFound = 0;
    std::string_view splitted;
    std::vector<const char*> resultados;

    while(posFound >= 0){
      posFound = str.find(pattern, posInit);
      splitted = str.substr(posInit, posFound - posInit);
      posInit = posFound + 1;
      resultados.push_back(splitted.data());
    }

    resultados.pop_back();

    return resultados;
  }

  int __stdcall WinMain(void*, void*, const char* c, int d)
  {
    auto args = splitArgs(c, ' ');
    args.insert(args.begin(), _pgmptr);

    main(args.size(), args.data());
    return 0;
  }
#endif
