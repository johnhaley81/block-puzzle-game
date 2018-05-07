module Component = {
  let component = ReasonReact.statelessComponent("Component");
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
};

let make = (~id, ~move, children) =>
  Styletron.React.makeStyledComponent(
    ~rule=
      _props => BsCssCore.Css.(style([flexGrow(1), flexBasis(rem(10.))])),
    ~component=Component.component,
    ~make=Component.make(~id, ~move),
    children,
  );
