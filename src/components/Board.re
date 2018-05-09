let baseStyles =
  Styles.makeStylesFromProps(((rows, cols)) =>
    Theme.(
      Css.[
        backgroundColor(Colors.board),
        borderTop(Border.boardTopBottomSize, `solid, Border.color),
        borderBottom(Border.boardTopBottomSize, `solid, Border.color),
        borderLeft(Border.boardLeftRightSize, `solid, Border.color),
        borderRight(Border.boardLeftRightSize, `solid, Border.color),
        borderRadius(Border.boardRadius),
        display(`flex),
        flexDirection(`column),
        flexWrap(`wrap),
        height(`em(baseTileSizeMultiplier *. rows)),
        width(`em(baseTileSizeMultiplier *. cols)),
        margin(`em(0.5)),
      ]
    )
  );

let component = ReasonReact.statelessComponent("Board");

let make =
    (
      ~board: GameLogic.Board.t,
      ~cols,
      ~rows,
      ~onTileClick,
      ~styles=(_) => [],
      _children,
    ) => {
  ...component,
  render: _self =>
    <div
      className=(
        baseStyles(styles, (rows |> float_of_int, cols |> float_of_int))
      )>
      (
        board
        |. Belt.List.mapWithIndex((rowNumber, tiles) =>
             <BoardRow
               onTileClick
               tiles
               rowNumber
               key=(rowNumber |> string_of_int)
             />
           )
        |> Belt.List.toArray
        |> ReasonReact.array
      )
    </div>,
};
