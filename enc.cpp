#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

int main (int argc, char * argv[]){
  if (argc > 1 && strncmp(argv[1], "-h", 2) == 0){
    cout << "Encrypt a file using XOR encryption\n\n";
    cout << "proper syntax : " << argv[0] << " -h -d -v -k=[key] -f=[h|c|d] -o=[filename] [data]\n\n";
    cout << " OPTIONS           DESCRIPTION\n -------           -----------\n";
    cout << " -h  :             Display help\n\n";
    cout << " -d  :             Decrypt instead of encrypt. format modifiers are applied\n"; 
    cout << "                   to input instead of output\n\n";
    cout << " -v  :             Be verbose with output\n\n";
    cout << " -k=[key] :        Only mandatory option, unless -h is specified\n";
    cout << "                   [key] specifies the key to be used for Encrypion\n\n";
    cout << " -f=[h|c|d] :      Specify the format to output the encrypted data\n";
    cout << "                   h == Hex output\n";
    cout << "                   c == Character output\n";
    cout << "                   d == Decimal output\n\n";
    cout << " -o=[filename] :   Output to file [filename] instead of standard output\n\n";
    cout << " [data] :          This is the data to Encrypt, therefore it is mandatory.\n";
    cout << "                   Every string that is passed to enc will be encrypted.\n";
    cout << "                   Strings are seperated by blank spaces so remember to\n";
    cout << "                   put data containing spaces inside quotation marks (\"\").\n";
    cout << "                   Data can be piped into enc, but it is necessary to use\n";
    cout << "                   xargs. long strings with spaces should be null terminated,\n";
    cout << "                   and the -0 or --null xargs option should be used.\n";
    cout << "  EXAMPLES\n  --------\n\n";
    cout << "  enc -k=secret_key -f=d -o=EncryptedFile.txt \"Hello world!\"\n\n";
    cout << "  echo -e \"Hello world!\\0\" | xargs -0 enc -k=secret_key\n";	
  }
  else if (argc < 3){
    cout << "proper syntax : " << argv[0] << " -h -d -v -k=[key] -f=[h|c|d] -o=[filename] [data]" << endl;
  }
  else{
    uint8_t args = 6;
    bool kflg = false;
    bool fflg = false;
    bool oflg = false;
    bool vflg = false;
    bool dflg = false;

    uint8_t keylen;
    char key[32];

    char format;

    uint8_t fnlen;
    char fname[32];

    uint8_t dstrt = 1;
    ofstream file;

    if (argc < 7){
      args = argc - 1;
    }

    for (int i = 1; i < args; i++){
      if (strncmp(argv[i], "-k=", 3) == 0){
        keylen = strlen(argv[i]) - 3;
        strncpy(key, argv[i] + 3, keylen);
        key[keylen] = '\0';
        kflg = true;
        dstrt++;
        if (vflg == true){
	  cout << "Encrypting data with key \"" << key << "\"\n";
	}
      }
      else if (strncmp(argv[i], "-f=", 3) == 0){
        format = argv[i][3];
        fflg = true;
        dstrt++;
	if (vflg == true){
	  cout << "Using format modifier \"" << format << "\"\n";
	  switch (format){
	    case 'd':
              cout << "Data will be output in decimal format\n";
              break;
            case 'c':
              cout << "Data will be output in character format\n";
              break;
            case 'h':
              cout << "Data will be output in hexadecimal format\n";
              break;
          }
	}
      }
      else if (strncmp(argv[i], "-o=", 3) == 0){
        fnlen = strlen(argv[i]) - 3;
        strncpy(fname, argv[i] + 3, fnlen);
        fname[fnlen] = '\0';
        oflg = true;
        dstrt++;
		if (vflg == true){
		  cout << "File name for data output \"" << fname << "\"\n";
		}
      }
      else if (strncmp(argv[i], "-v", 2) == 0){
        vflg = true;
        dstrt++;
      }
      else if (strncmp(argv[i], "-d", 2) == 0){
        dflg = true;
        dstrt++;
      }
    }
    if (kflg != true){
      cout << "-k=[key] parameter required" << endl;
    }
    else if (dflg == true){
      if (oflg == true){
        file.open(string(fname));
      }
      int tmpn;
      char tmp;
      for (int i = dstrt; i < argc; i++){
        if (fflg == true){
          switch (format){
            case 'd':
              tmpn = (int)strtoul(argv[i], NULL, 0);
              tmp = tmpn ^ key[(i - dstrt) % keylen];
              if (oflg == true){
                file << tmp;
              }
              else{
                cout << tmp;
              }
              break;
            case 'c':
              tmp = argv[i][0] ^ key[(i - dstrt) % keylen];
              if (oflg == true){
                file << tmp;
              }
              else{
                cout << tmp;
              }
              break;
            case 'h':
              tmpn = (int)strtoul(argv[i], NULL, 16);
              tmp = tmpn ^ key[(i - dstrt) % keylen];
              if (oflg == true){
                file << tmp;
              }
              else{
                cout << tmp;
              }
              break;
          }
        }
        else {
          tmpn = (int)strtoul(argv[i], NULL, 16);
          tmp = tmpn ^ key[(i - dstrt) % keylen];
          if (oflg == true){
            file << tmp;
          }
          else{
            cout << tmp;
          }
        }
      }
      if (oflg == true){
        file.close();
      }
    }
    else {
      size_t count = 0;
      if (oflg == true){
        file.open(string(fname));
      }
      for (int i = dstrt; i < argc; i++){
        for  (int o = 0; o < strlen(argv[i]); o++){
          char tmp = argv[i][o];
          int tmpn = tmp ^ key[count % keylen];
          if (fflg == true){
            switch (format){
              case 'c':
                if (oflg == true){
                  file << (char)tmpn;
                  file << ' ';
                }
                else{
                  cout << (char)tmpn;
                  cout << ' ';
                }
                break;
              case 'd':
                if (oflg == true){
                  file << tmpn;
                  file << ' ';
                }
                else {
                  cout << tmpn;
                  cout << ' ';
                }
                break;
              case 'h':
                if (oflg == true){
                  file << setfill('0') << setw(2) << hex << tmpn;
                  file << ' ';
                }
                else {
                  cout << setfill('0') << setw(2) << hex << tmpn;
                  cout << ' ';
                }
                break;
              default :
                cout << "Wrong format specified in -f option" << endl;
                break;
            }
          }
          else{ 
            if (vflg == true){
              cout << "Data will be output in hexadecimal format\n";
            }
            if (oflg == true){
              file << setfill('0') << setw(2) << hex << tmpn;
              cout << ' ';
            }
            else {
              cout << setfill('0') << setw(2) << hex << tmpn;
              cout << ' ';
            }
          }
          count++;
        }
      }
      cout << endl;
      if (oflg == true){
        file.close();
      }
    }
  }
  return 0;
}
