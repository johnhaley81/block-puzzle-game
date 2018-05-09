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
  let triangleRotationStyles = move =>
    GameLogic.(
      Css.(
        (
          switch (move) {
          | NoMove
          | Up => deg(0)
          | Right => deg(90)
          | Down => deg(180)
          | Left => deg(270)
          }
        )
        |> (
          rotation => [
            height(`percent(100.0)),
            width(`percent(100.0)),
            transform(`rotate(rotation)),
          ]
        )
        |> style
      )
    );
  let triangleRawSize = 0.3125;
  let triangleSize = `em(triangleRawSize);
  let triangleSharedStyles =
    Theme.(
      Css.[
        height(triangleSize),
        width(triangleSize),
        borderTopRightRadius(`percent(30.0)),
        backgroundColor(Colors.blockMovePointer),
      ]
    );
  let triangleSharedPseudoStyles =
    Theme.(
      Css.(
        merge([
          [`declaration(("content", "''")), position(`absolute)],
          triangleSharedStyles,
        ])
      )
    );
  let triangleAfterStyles =
    Theme.(
      Css.(
        merge([
          [
            transforms([
              `rotate(deg(135)),
              `skewY(deg(-45)),
              `scale((0.707, 1.414)),
              translate(`percent(50.0), `zero),
            ]),
          ],
          triangleSharedPseudoStyles,
        ])
      )
    );
  let triangleBeforeStyles =
    Theme.(
      Css.(
        merge([
          [
            transforms([
              `rotate(deg(-135)),
              `skewX(deg(-45)),
              `scale((1.414, 0.707)),
              translate(`zero, `percent(-50.0)),
            ]),
          ],
          triangleSharedPseudoStyles,
        ])
      )
    );
  let triangleStyles =
    Theme.(
      Css.(
        style(
          merge([
            [
              left(
                `calc((`sub, `percent(50.0), `em(triangleRawSize /. 2.0))),
              ),
              top(`em(-. triangleRawSize)),
              position(`relative),
              textAlign(`left),
              transforms([
                `rotate(deg(-60)),
                `skewX(deg(-30)),
                `scale((1.0, 0.866)),
              ]),
              after(triangleAfterStyles),
              before(triangleBeforeStyles),
            ],
            triangleSharedStyles,
          ]),
        )
      )
    );
  let component = ReasonReact.statelessComponent("MovementIndicator");
  let make = (~move: GameLogic.move, ~styles=[], _children) => {
    ...component,
    render: _self =>
      <div className=(makeStyles(styles))>
        <div className=(triangleRotationStyles(move))>
          (
            move == GameLogic.NoMove ?
              ReasonReact.null : <div className=triangleStyles />
          )
        </div>
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
