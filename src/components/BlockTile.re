module MovementIndicator = {
  let makeStyles =
    Styles.makeStyles(
      Theme.(
        Css.[
          flexGrow(1),
          margin(`em(0.25)),
          border(`em(0.1875), Border.style, Border.color),
          borderRadius(`em(0.375)),
        ]
      ),
    );
  let triangleSize = `em(0.3125);
  let triangle = (_) =>
    Theme.(
      Css.(
        style([
          position(`relative),
          before([
            `declaration(("content", "''")),
            position(`absolute),
            height(`zero),
            width(`zero),
            borderTop(triangleSize, Border.style, `transparent),
            borderBottom(triangleSize, Border.style, `transparent),
            borderRight(triangleSize, Border.style, Colors.blockMovePointer),
            borderRadius(triangleSize),
          ]),
        ])
      )
    );
  let component = ReasonReact.statelessComponent("MovementIndicator");
  let make = (~move: GameLogic.move, ~styles=[], _children) => {
    ...component,
    render: _self =>
      <div className=(makeStyles(styles))>
        <div className=(triangle(move)) />
      </div>,
  };
};

let makeStyles =
  Styles.makeStyles(
    Theme.(
      Css.[
        backgroundColor(Colors.blockBase),
        display(`flex),
        flexGrow(1),
        margin(`em(0.1875)),
        borderRadius(`em(0.375)),
      ]
    ),
  );

let component = ReasonReact.statelessComponent("BlockTile");

let make = (~id, ~move: GameLogic.move, ~styles=[], _children) => {
  ...component,
  render: _self =>
    <div className=(makeStyles(styles)) key=(id |> string_of_int)>
      <MovementIndicator move />
    </div>,
};
