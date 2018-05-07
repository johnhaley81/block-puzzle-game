let _base = () => {
  let component = ReasonReact.statelessComponent("EndTile");
  let make = (~className, _children) => {
    ...component,
    render: _self => <div className> ("E" |> ReasonReact.string) </div>,
  };
  (component, make);
};

let make = children =>
  _base()
  |> (
    ((component, make)) =>
      Styletron.React.makeStyledComponent(
        ~rule=_props => BsCssCore.Css.(style([flexGrow(1)])),
        ~component,
        ~make,
        children,
      )
  );
