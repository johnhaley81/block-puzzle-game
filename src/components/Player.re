let _base = () => {
  let component = ReasonReact.statelessComponent("Player");
  let make = (~className, _children) => {
    ...component,
    render: _self => <div className> ("P" |> ReasonReact.string) </div>,
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
