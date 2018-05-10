type move =
  | NoMove
  | Left
  | Right
  | Up
  | Down;

type boardPiece =
  | Empty
  | Start(bool)
  | End(bool)
  | Block(int, move)
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
  | Left => {row, col: col - 1}
  | Right => {row, col: col + 1}
  | Up => {row: row - 1, col}
  | Down => {row: row + 1, col}
  };

let moveFromPositions =
    (
      {row: fromRow, col: fromCol}: Position.t,
      {row: toRow, col: toCol}: Position.t,
    ) =>
  switch (toRow - fromRow, toCol - fromCol) {
  | (0, (-1)) => Left
  | (0, 1) => Right
  | ((-1), 0) => Up
  | (1, 0) => Down
  | _ => NoMove
  };

let rec getMoves = (offset, t) =>
  switch (t) {
  | [] => []
  | [Block(id, move), ...rest] => [
      (offset, Block(id, move)),
      ...getMoves(offset + 1, rest),
    ]
  | [Player, ...rest] => [(offset, Player), ...getMoves(offset + 1, rest)]
  | [Start(containsPlayer), ...rest] => [
      (offset, Start(containsPlayer)),
      ...getMoves(offset + 1, rest),
    ]
  | [End(containsPlayer), ...rest] => [
      (offset, End(containsPlayer)),
      ...getMoves(offset + 1, rest),
    ]
  | [Empty, ...rest] => getMoves(offset + 1, rest)
  };

let getMoves = getMoves(0);

let canMoveIntoTile = (adjacentTiles, boardPiece) =>
  switch (adjacentTiles, boardPiece) {
  | ({left: Some(Block(_, Right)), _}, _)
  | ({left: Some(Start(true)), _}, _)
  | ({left: Some(End(true)), _}, _)
  | ({right: Some(Block(_, Left)), _}, _)
  | ({up: Some(Block(_, Down)), _}, _)
  | ({down: Some(Block(_, Up)), _}, _)
  | ({center: None, _}, _)
  | ({center: Some(End(_))}, Block(_, _))
  | ({center: Some(Start(_)), _}, _)
  | ({center: Some(Player), _}, _)
  | ({center: Some(Block(_, NoMove))}, _) => false
  | ({left: Some(Block(_, Left))}, _)
  | ({left: Some(Block(_, Up))}, _)
  | ({left: Some(Block(_, Down))}, _)
  | ({left: Some(Block(_, NoMove))}, _)
  | ({left: Some(Empty)}, _)
  | ({left: Some(Start(false))}, _)
  | ({left: Some(End(false))}, _)
  | ({left: Some(Player)}, _)
  | ({left: None}, _) => true
  };

module Board = {
  type t = list(list(boardPiece));
  let rows = 10;
  let cols = 6;
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
           | Block(id, _) => updateBoardForPiece(Block(id, NoMove))
           | Player => updateBoardForPiece(Player)
           | Empty => t
           | Start(containsPlayer) =>
             updateBoardForPiece(Start(containsPlayer))
           | End(containsPlayer) => updateBoardForPiece(End(containsPlayer))
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
    Belt.List.concat([], Belt.List.makeBy(blocks, id => Block(id, NoMove)))
    |. Belt.List.reduce(
         [
           (Position.make(0, 0), End(false)),
           (Position.make(rows - 1, cols - 1), Start(true)),
         ],
         (currentPositionsForPieces, boardPiece) =>
         switch (
           currentPositionsForPieces
           |> getCurrentlyOccupiedPositions
           |> getOpenPosition(_, rows, cols),
           boardPiece,
         ) {
         | (None, _) => currentPositionsForPieces
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
  let canMove = (move, position, boardPiece, t) =>
    movePosition(move, position)
    |> (position => getAdjacentTiles(position, t))
    |> canMoveIntoTile(_, boardPiece);
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
  let getPiecePositionOnBoard = (boardPieceToFind, t) =>
    t
    |> reduceBoard(
         None,
         (result, next) =>
           switch (result, next) {
           | (Some(x), Some(_))
           | (Some(x), None)
           | (None, Some(x)) => Some(x)
           | (None, None) => None
           },
         None,
         (result, (position, boardPiece)) =>
           (
             switch (boardPiece, boardPieceToFind) {
             | (End(_), End(_))
             | (Start(_), Start(_))
             | (End(true), Player)
             | (Start(true), Player)
             | (Player, Player) => Some(position)
             | (Block(id, _), Block(idToFind, _)) when id == idToFind =>
               Some(position)
             | _ => None
             }
           )
           |. Belt.Option.mapWithDefault(result, x => Some(x)),
       );
  let collectOverBoard = reduceBoard([], Belt.List.concat, []);
  let collectAllBoardPiecePositions =
    collectOverBoard((results, (_, boardPiece) as piecePosition) =>
      switch (boardPiece) {
      | Block(_, _)
      | Player
      | Start(_)
      | End(_) => [piecePosition, ...results]
      | Empty => results
      }
    );
  let collectAllMoves = (playerMove, t) =>
    collectOverBoard(
      (results, (position, boardPiece)) =>
        {
          let movePositionForPiece = movePosition(_, position);
          switch (boardPiece) {
          | Block(_, move) => (
              (movePositionForPiece(move), boardPiece),
              None,
            )
          | Start(true) when playerMove == NoMove => (
              (position, Start(true)),
              None,
            )
          | Start(true) => (
              (movePositionForPiece(playerMove), Player),
              Some((movePositionForPiece(NoMove), Start(false))),
            )
          | End(true) when playerMove == NoMove => (
              (position, End(true)),
              None,
            )
          | End(true) => (
              (movePositionForPiece(playerMove), Player),
              Some((movePositionForPiece(NoMove), End(false))),
            )
          | Player => ((movePositionForPiece(playerMove), Player), None)
          | Empty
          | Start(false)
          | End(false) => (
              (movePositionForPiece(NoMove), boardPiece),
              None,
            )
          };
        }
        |> (
          ((newPosition, optionalPosition)) =>
            switch (optionalPosition) {
            | Some(optionalPosition) => [
                newPosition,
                optionalPosition,
                ...results,
              ]
            | None => [newPosition, ...results]
            }
        ),
      t,
    );
  let rec getAllValidMovesForTile = (move, position, boardPiece, t) => {
    let getNextValidMovesForTile =
      getAllValidMovesForTile(_, position, boardPiece, t);
    switch (move, canMove(move, position, boardPiece, t)) {
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
  let getAllValidMovesForTile = (position, boardPiece, t) =>
    getAllValidMovesForTile(Down, position, boardPiece, t);
  let getRandomValidMoveForTile = (position, boardPiece, t) =>
    switch (
      getAllValidMovesForTile(position, boardPiece, t)
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
         /* We're only updating blocks with no moves */
         | Block(_, NoMove) => [(position, boardPiece), ...results]
         | Block(_, _)
         | Player
         | Empty
         | Start(_)
         | End(_) => results
         }
       )
    |. Belt.List.reduce(t, (t, boardPiecePosition) =>
         switch (boardPiecePosition) {
         | (position, Block(id, NoMove)) =>
           getRandomValidMoveForTile(position, Block(id, NoMove), t)
           |> (move => updateBoardPiece(Block(id, move), position, t))
         | _ => t
         }
       );
  let doTurn = (rows, cols, playerMove, t) =>
    /* Don't do a turn if the player isn't moving */
    playerMove == NoMove ?
      t :
      t
      |> collectAllMoves(playerMove)
      |> createBoardWithMoves(rows, cols)
      |> updateBlockMovesForNextTurn;
};
