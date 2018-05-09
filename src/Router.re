type route =
  | PlayGame;

type state = {route};

type action =
  | ChangeRoute(route);

let reducer = (action, _state) =>
  switch (action) {
  | ChangeRoute(route) => ReasonReact.Update({route: route})
  };

let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | _ => PlayGame
  };

let component = ReasonReact.reducerComponent("Router");

let make = _children => {
  ...component,
  reducer,
  initialState: () => {
    route: ReasonReact.Router.dangerouslyGetInitialUrl() |> mapUrlToRoute,
  },
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url =>
          self.send(ChangeRoute(url |> mapUrlToRoute))
        ),
      ReasonReact.Router.unwatchUrl,
    ),
  ],
  render: self =>
    <div>
      <main>
        (
          switch (self.state.route) {
          | PlayGame => <PlayGame />
          }
        )
      </main>
    </div>,
};
