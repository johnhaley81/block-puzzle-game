module Component = {
  let component = ReasonReact.statelessComponent("Component");
  let make = (~className, _children) => {
    ...component,
    render: _self => <div className> ("E" |> ReasonReact.string) </div>,
  };
};

let make = children =>
  Styletron.React.makeStyledComponent(
    ~rule=
      _props => BsCssCore.Css.(style([flexGrow(1), flexBasis(rem(10.))])),
    ~component=Component.component,
    ~make=Component.make,
    children,
  );
