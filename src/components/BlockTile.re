let _base = () => {
  let component = ReasonReact.statelessComponent("BlockTile");
  let make = (~id, ~move: GameLogic.move, ~className, _children) => {
    ...component,
    render: _self =>
      <div className key=(id |> string_of_int)>
        (
          [|
            switch (move) {
            | NoMove
            | Left
            | Right
            | Up
            | Down => ReasonReact.null
            },
            ReasonReact.string("B"),
          |]
          |> ReasonReact.array
        )
      </div>,
  };
  (component, make);
};

let make = (~id, ~move, children) =>
  _base()
  |> (
    ((component, make)) =>
      Styletron.React.makeStyledComponent(
        ~rule=_props => BsCssCore.Css.(style([flexGrow(1)])),
        ~component,
        ~make=make(~id, ~move),
        children,
      )
  );
