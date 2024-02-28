#ifndef BPT_UTILS_HPP
#define BPT_UTILS_HPP

#include <fstream>

using std::fstream;

namespace CrazyDave {
    template<size_t len>
    class String {
        // 可写字符串类
        char str[len]{'\0'};
    public:
        String() = default;

        String(const char *s) {
            strcpy(str, s);
        }

        String(const std::string &s) {
            strcpy(str, s.c_str());
        }

        explicit operator const char *() {
            return str;
        }

        operator const std::string() {
            return std::move(std::string(str));
        }

        const char *c_str() {
            return str;
        }

        char &operator[](int pos) {
            return str[pos];
        }

        String &operator=(const String &rhs) {
            if (this == &rhs)return *this;
            strcpy(str, rhs.str);
            return *this;
        }

        String &operator=(const char *s) {
            strcpy(str, s);
            return *this;
        }

        String &operator=(const std::string &s) {
            strcpy(str, s.c_str());
            return *this;
        }


        bool operator==(const String &rhs) const {
            return !strcmp(str, rhs.str);
        }

        bool operator!=(const String &rhs) const {
            return strcmp(str, rhs.str);
        }

        bool operator<(const String &rhs) const {
            return strcmp(str, rhs.str) < 0;
        }

        friend int cmp_str(const String &lhs, const String &rhs) {
            return strcmp(lhs.str, rhs.str);
        }

        friend std::istream &operator>>(std::istream &is, String &rhs) {
            return is >> rhs.str;
        }

        friend std::ostream &operator<<(std::ostream &os, const String &rhs) {
            return os << rhs.str;
        }
    };

    class File {
        char name[65]{'\0'};
        fstream fs;
        bool is_new = false;
    public:
        explicit File(const char *_name) {
            strcpy(name, _name);
            fs.open(name, std::ios::in);
            if (!fs) {
                fs.open(name, std::ios::out);
                is_new = true;
            }
            fs.close();
        }

        void open(std::ios::openmode mode = std::ios::in | std::ios::out) {
            fs.open(name, mode);
        }

        void close() {
            fs.close();
        }

        bool get_is_new() const {
            return is_new;
        }

        template<class T>
        void read(T &dst, size_t size = sizeof(T)) {
            fs.read(reinterpret_cast<char *>(&dst), (long) size);
        }

        template<class T>
        void write(T &src, size_t size = sizeof(T)) {
            fs.write(reinterpret_cast<const char *>(&src), (long) size);
        }

        void seekg(int pos) {
            fs.seekg(pos);
        }

        void seekp(int pos) {
            fs.seekp(pos);
        }
    };

    template<class T>
    int cmp(const T &lhs, const T &rhs) {
        if (lhs < rhs)return -1;
        else return rhs < lhs;
    }

    template<class T>
    int binary_search(const T *first, int len, const T &val) {
        int l = 0, r = len;
        while (l <= r) {
            int mid = (l + r) >> 1;
            int flag = cmp(first[mid], val);
            if (flag < 0) {
                l = mid + 1;
            } else if (flag > 0) {
                r = mid - 1;
            } else {
                return mid;
            }
        }
        return -1;
    }

    template<class T1, class T2>
    int upper_bound(const T1 *first, int len, const T2 &val) {
        // Find the first element that > val.
        int l = 0, r = len - 1;
        int res = len;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (val < first[mid]) {
                res = mid;
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }
        return res;
    }

    template<class T1, class T2>
    int lower_bound(const T1 *first, int len, const T2 &val) {
        // Find the first element that >= val.
        int l = 0, r = len - 1;
        int res = len;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (!(first[mid] < val)) {
                res = mid;
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }
        return res;
    }

    template<class T>
    void insert_at(T *first, int len, int pos, const T &val) {
        for (int i = len; i > pos; --i) {
            first[i] = first[i - 1];
        }
        first[pos] = val;
    }

    template<class T>
    void remove_at(T *first, int len, int pos) {
        for (int i = pos; i < len - 1; ++i) {
            first[i] = first[i + 1];
        }
    }

    template<class T>
    int increase_insert(T *first, int len, const T &val) {
        int pos = lower_bound(first, len, val);
        insert_at(first, len, pos, val);
        return pos;
    }

    template<class T>
    bool increase_remove(T *first, int len, const T &val) {
        int pos = binary_search(first, len, val);
        if (pos != -1) {
            remove_at(first, len, pos);
            return true;
        }
        return false;
    }

}
#endif //BPT_UTILS_HPP
