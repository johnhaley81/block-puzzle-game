type move =
  | NoMove
  | Left
  | Right
  | Up
  | Down;

type boardPiece =
  | Empty
  | Start
  | End
  | Block(move)
  | Player;

type adjacentTiles = {
  left: option(boardPiece),
  right: option(boardPiece),
  up: option(boardPiece),
  down: option(boardPiece),
  center: option(boardPiece),
};

module Position = {
  type t = {
    row: int,
    col: int,
  };
  let make = (row, col) => {row, col};
  let makeRandom = (rows, cols) =>
    make(Random.int(rows), Random.int(cols));
  let cmp = (p1, p2) =>
    hypot(p2.row - p1.row |> float_of_int, p2.col - p1.col |> float_of_int)
    |> Pervasives.int_of_float;
};

let getElement = (offset, t) =>
  t |. Belt.List.drop(offset) |. Belt.Option.flatMap(Belt.List.head);

let rec updateElement = (newElement, offset, t) =>
  switch (offset, t) {
  | (0, [_, ...rest]) => [newElement, ...rest]
  | (_, []) => []
  | (offset, [element, ...rest]) => [
      element,
      ...updateElement(newElement, offset - 1, rest),
    ]
  };

let movePosition = (move, {row, col}: Position.t) : Position.t =>
  switch (move) {
  | NoMove => {row, col}
  | Left => {row: row - 1, col}
  | Right => {row: row + 1, col}
  | Up => {row, col: col - 1}
  | Down => {row, col: col + 1}
  };

let rec getMoves = (offset, t) =>
  switch (t) {
  | [] => []
  | [Block(move), ...rest] => [
      (offset, Block(move)),
      ...getMoves(offset + 1, rest),
    ]
  | [Player, ...rest] => [(offset, Player), ...getMoves(offset + 1, rest)]
  | [Start, ...rest] => [(offset, Start), ...getMoves(offset + 1, rest)]
  | [End, ...rest] => [(offset, End), ...getMoves(offset + 1, rest)]
  | [Empty, ...rest] => getMoves(offset + 1, rest)
  };

let getMoves = getMoves(0);

let canMoveIntoTile = adjacentTiles =>
  switch (adjacentTiles) {
  | {left: Some(Block(Right))}
  | {right: Some(Block(Left))}
  | {up: Some(Block(Down))}
  | {down: Some(Block(Up))}
  | {center: Some(Block(NoMove))} => false
  | {left: Some(Block(Left))}
  | {left: Some(Block(Up))}
  | {left: Some(Block(Down))}
  | {left: Some(Block(NoMove))}
  | {left: Some(Empty)}
  | {left: Some(Start)}
  | {left: Some(End)}
  | {left: Some(Player)}
  | {left: None} => true
  };

module Board = {
  type t = list(list(boardPiece));
  let rows = 7;
  let cols = 11;
  let blocks = 20;
  let makeEmptyBoard = (rows, cols) =>
    Belt.List.makeBy(rows, (_) => Belt.List.make(cols, Empty));
  let rec updateBoardPiece = (newBoardPiece, {row, col}: Position.t, t) =>
    switch (row, t) {
    | (0, [rowToUpdate, ...rest]) => [
        updateElement(newBoardPiece, col, rowToUpdate),
        ...rest,
      ]
    | (_, []) => []
    | (row, [rowToKeep, ...rest]) => [
        rowToKeep,
        ...updateBoardPiece(newBoardPiece, {row: row - 1, col}, rest),
      ]
    };
  let createBoardWithMoves = (rows, cols, moves) =>
    moves
    |. Belt.List.reduce(
         makeEmptyBoard(rows, cols),
         (t, (position, boardPiece)) => {
           let updateBoardForPiece = updateBoardPiece(_, position, t);
           switch (boardPiece) {
           | Block(_) => updateBoardForPiece(Block(NoMove))
           | Player => updateBoardForPiece(Player)
           | Empty => t
           | Start => updateBoardForPiece(Start)
           | End => updateBoardForPiece(End)
           };
         },
       );
  let rec getOpenPosition = (occupiedPositions, rows, cols) =>
    switch (
      Position.make(Random.int(rows), Random.int(cols)),
      Belt.Set.size(occupiedPositions) == rows * cols,
    ) {
    | (_, true) => None
    | (pos, false) when Belt.Set.has(occupiedPositions, pos) =>
      getOpenPosition(occupiedPositions, rows, cols)
    | (pos, false) => Some(pos)
    };
  let getCurrentlyOccupiedPositions = currentPositionsForPieces =>
    currentPositionsForPieces
    |. Belt.List.map(((position, _)) => position)
    |> Belt.List.toArray
    |> Belt.Set.fromArray(~id=(module Belt.Id.MakeComparable(Position)));
  let createStartingPositionsForPieces = (rows, cols, blocks) =>
    Belt.List.concat([Start, End], Belt.List.make(blocks, Block(NoMove)))
    |. Belt.List.reduce([], (currentPositionsForPieces, boardPiece) =>
         switch (
           currentPositionsForPieces
           |> getCurrentlyOccupiedPositions
           |> getOpenPosition(_, rows, cols),
           boardPiece,
         ) {
         | (None, _) => currentPositionsForPieces
         | (Some(position), Start) => [
             (position, Start),
             (position, Player),
             ...currentPositionsForPieces,
           ]
         | (Some(position), boardPiece) => [
             (position, boardPiece),
             ...currentPositionsForPieces,
           ]
         }
       );
  let createBoard = (rows, cols, blocks) =>
    createStartingPositionsForPieces(rows, cols, blocks)
    |> createBoardWithMoves(rows, cols);
  let getBoardPiece = ({row, col}: Position.t, t) =>
    t |> getElement(row) |. Belt.Option.flatMap(getElement(col));
  let getBoardPieceForMove = (move, position, t) =>
    movePosition(move, position) |> (position => getBoardPiece(position, t));
  let getAdjacentTiles = (position, t) => {
    left: getBoardPieceForMove(Left, position, t),
    right: getBoardPieceForMove(Right, position, t),
    up: getBoardPieceForMove(Up, position, t),
    down: getBoardPieceForMove(Down, position, t),
    center: getBoardPieceForMove(NoMove, position, t),
  };
  let canMove = (move, position, t) =>
    movePosition(move, position)
    |> (position => getAdjacentTiles(position, t))
    |> canMoveIntoTile;
  let reduceBoard = (initRow, rowFn, initCol, colFn, t) =>
    Belt.List.reduce(t, (0, initRow), ((rowNum, resultsSoFar), row) =>
      (
        rowNum + 1,
        rowFn(
          resultsSoFar,
          Belt.List.reduce(
            row, (0, initCol), ((colNum, resultsSoFar), boardPiece) =>
            (
              colNum + 1,
              colFn(
                resultsSoFar,
                ({row: rowNum, col: colNum}: Position.t, boardPiece),
              ),
            )
          )
          |> (((_, results)) => results),
        ),
      )
    )
    |> (((_, results)) => results);
  let collectOverBoard = reduceBoard([], Belt.List.concat, []);
  let collectAllMoves = (playerMove, t) =>
    collectOverBoard(
      (results, (position, boardPiece)) =>
        {
          let movePositionForPiece = movePosition(_, position);
          switch (boardPiece) {
          | Block(move) => movePositionForPiece(move)
          | Player => movePositionForPiece(playerMove)
          | Empty
          | Start
          | End => movePositionForPiece(NoMove)
          };
        }
        |> (position => [(position, boardPiece), ...results]),
      t,
    );
  let rec getAllValidMovesForTile = (move, position, t) => {
    let getNextValidMovesForTile = getAllValidMovesForTile(_, position, t);
    switch (move, canMove(move, position, t)) {
    /* NoMove is added iff there are no valid moves. That is checked elsewhere */
    | (NoMove, _) => []
    | (Left, true) => [Left, ...getNextValidMovesForTile(NoMove)]
    | (Left, false) => getNextValidMovesForTile(NoMove)
    | (Right, true) => [Right, ...getNextValidMovesForTile(Left)]
    | (Right, false) => getNextValidMovesForTile(Left)
    | (Up, true) => [Up, ...getNextValidMovesForTile(Right)]
    | (Up, false) => getNextValidMovesForTile(Right)
    | (Down, true) => [Down, ...getNextValidMovesForTile(Up)]
    | (Down, false) => getNextValidMovesForTile(Up)
    };
  };
  let getAllValidMovesForTile = (position, t) =>
    getAllValidMovesForTile(Down, position, t);
  let getRandomValidMoveForTile = (position, t) =>
    switch (
      getAllValidMovesForTile(position, t)
      |> Belt.List.shuffle
      |> Belt.List.head
    ) {
    | Some(move) => move
    | None => NoMove
    };
  let updateBlockMovesForNextTurn = t =>
    t
    |> collectOverBoard((results, (position, boardPiece)) =>
         switch (boardPiece) {
         | Block(NoMove) => [(position, boardPiece), ...results]
         | Block(_)
         | Player
         | Empty
         | Start
         | End => results
         }
       )
    |. Belt.List.reduce(t, (t, (position, _)) =>
         getRandomValidMoveForTile(position, t)
         |> (move => updateBoardPiece(Block(move), position, t))
       );
  let doTurn = (rows, cols, playerMove, t) =>
    t
    |> collectAllMoves(playerMove)
    |> createBoardWithMoves(rows, cols)
    |> updateBlockMovesForNextTurn;
};
