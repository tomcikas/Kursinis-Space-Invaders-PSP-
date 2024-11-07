#pragma once

class Score {
public:
    Score();
    void addPoints(int points);
    int getScore() const;
    void reset();

private:
    int score;
};

