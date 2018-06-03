#ifndef PCRE_REGEX_HPP
#define PCRE_REGEX_HPP

#include <pcre.h>
#include <string>
#include <list>

namespace mycpp {

    class pcre_regex {
    public:
        pcre_regex() = delete;

        pcre_regex(const std::string& pattern) : reg(0), reg_extra(0) {
            const char *error;
            int erroffset;
            this->reg = pcre_compile(pattern.c_str(), 0, &error, &erroffset, NULL);
            if (!this->reg) {
                this->reg_extra = pcre_study(this->reg, 0, &error);
            }
        }

        virtual~pcre_regex() {
            if (this->reg)pcre_free(this->reg);
            if (this->reg_extra)pcre_free_study(this->reg_extra);
        }

        bool match(const std::string& subject, std::list<std::string>& matches, size_t n = 30) {
            bool result = false;
            int ovector[n];
            int rc = pcre_exec(this->reg, this->reg_extra, subject.c_str(), subject.size(), 0, 0, ovector, n);
            if (rc >= 0) {
                result = true;
                int start, len;
                for (int i = 0; i < rc; i++) {
                    start = ovector[2 * i];
                    len = ovector[2 * i + 1] - start;
                    matches.push_back(std::move(subject.substr(start, len)));
                }

            }
            return result;
        }

    private:
        pcre* reg;
        pcre_extra *reg_extra;
    };
}

#endif /* PCRE_REGEX_HPP */
