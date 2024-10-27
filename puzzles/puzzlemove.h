#ifndef PUZZLEMOVE_H
#define PUZZLEMOVE_H


class PuzzleMove {
  private:
    int _from;
    int _to;
    bool _puzzleStart;
    int _puzzleId;

  public:
    PuzzleMove(int from, int to, bool puzzleStart, int puzzleId)
      : _from(from), _to(to), _puzzleStart(puzzleStart), _puzzleId(puzzleId) {}

    int getFrom() const { return _from; }
    void setFrom(int from) { _from = from; }

    int getTo() const { return _to; }
    void setTo(int to) { _to = to; }

    bool isPuzzleStart() const { return _puzzleStart; }
    void setPuzzleStart(bool puzzleStart) { _puzzleStart = puzzleStart; }

    int getPuzzleId() const { return _puzzleId; }
    void setPuzzleId(int puzzleId) { _puzzleId = puzzleId; }
};


#endif // PUZZLEMOVE_H
