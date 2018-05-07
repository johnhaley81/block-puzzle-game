let component = ReasonReact.statelessComponent("StartTile");

let make = (~className, _children) => {
  ...component,
  render: _self => <div className> ("S" |> ReasonReact.string) </div>,
};

let make = children =>
  Styletron.React.makeStyledComponent(
    ~rule=_props => BsCssCore.Css.(style([flexGrow(1)])),
    ~component,
    ~make,
    children,
  );
