type state = {
  board: GameLogic.Board.t,
  turnCount: int,
  rows: int,
  cols: int,
  blocks: int,
};

type action =
  | DoMove(GameLogic.move);

let reducer = (action, {board, rows, cols, turnCount} as state) =>
  switch (action) {
  | DoMove(NoMove) => ReasonReact.NoUpdate
  | DoMove(move) =>
    board
    |> GameLogic.Board.getPiecePositionOnBoard(GameLogic.Player)
    |. Belt.Option.map(GameLogic.Board.canMove(move, _, board))
    |. Belt.Option.getWithDefault(false) ?
      ReasonReact.Update({
        ...state,
        board: board |> GameLogic.Board.doTurn(rows, cols, move),
        turnCount: turnCount + 1,
      }) :
      ReasonReact.NoUpdate
  };

let component = ReasonReact.reducerComponent("PlayGame");

let _onTileClick = (send, board, tileClickedPosition, _mouseClick) =>
  board
  |> GameLogic.Board.getPiecePositionOnBoard(GameLogic.Player)
  |. Belt.Option.map(GameLogic.moveFromPositions(_, tileClickedPosition))
  |. Belt.Option.getWithDefault(GameLogic.NoMove)
  |> (move => send(DoMove(move)));

let make =
    (
      ~rows=GameLogic.Board.rows,
      ~cols=GameLogic.Board.cols,
      ~blocks=GameLogic.Board.blocks,
      _children,
    ) => {
  ...component,
  reducer,
  initialState: () => {
    board:
      GameLogic.Board.createBoard(rows, cols, blocks)
      |> GameLogic.Board.updateBlockMovesForNextTurn,
    turnCount: 0,
    rows,
    cols,
    blocks,
  },
  render: self =>
    <Board
      board=self.state.board
      cols
      rows
      onTileClick=(_onTileClick(self.send, self.state.board))
    />,
};
