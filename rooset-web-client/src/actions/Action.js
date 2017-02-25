// @flow

import type { PageAction } from "./PageAction";
import type { QueryAction } from "./http/QueryActions";

export type Action = PageAction | QueryAction;
