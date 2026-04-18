#include <string>
#include <fstream>
#include <vector>

template<std::integral Int>
class highscore
{
  public:
    explicit highscore(std::string filename) : filename_(std::move(filename)){ }
    
    std::vector<std::pair<Int, std::string>> load_highscores()
    {
        std::ifstream ifs(filename_);
        if (!ifs.is_open())
            return {};
        std::vector<std::pair<int, std::string>> ret;
        int score;
        std::string name;
        while (ifs >> score >> name)
            ret.emplace_back(score, name);
        return ret;
    }
    
    void save_highscores(const std::vector<std::pair<Int, std::string>>& high_scores)
    {
        std::ofstream ofs(filename_);
        if (!ofs.is_open())
            return;
        for (auto& [score, name]: high_scores)
            ofs << score << ' ' << name << '\n';
    }
  
  private:
    std::string filename_;
};