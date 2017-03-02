// @flow

import type { PageAction } from "./PageAction";
import type { QueryAction } from "./http/QueryActions";
import type { SessionAction } from "./SessionAction";

export type HttpAction = QueryAction; // | CommandAction;
export type Action = PageAction | QueryAction | SessionAction;
